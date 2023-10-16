var Server = {
  server: undefined,
  connection_verified: false,
  in_online_game: false,
  requested_close: false,
  url_base: "",
  play_as: 'x'
};

function connectToServer(fromURLParam = false) {
  let ip = "";
  if (fromURLParam) {
    let search = location.search;
    if (search[0] != "?")
      return;
    ip = search.split("?")[1];
  }
  else
    ip = l("server-ip").value;
  Server.url_base = ip.split(/wss?\:\/\/[\/\?\#]/)[0];
  try {
    Server.server.close();
  } catch (e) { }
  try {
    Server.server = new WebSocket(ip);
    if (fromURLParam) {
      // zero ms timeout so that this is deferred long enough for the window
      // that we're trying to hide to pop up
      setTimeout(() => {
        hideWindows();
        showWindow("manage-connection");
      }, 0);
    }
  }
  catch (e) {
    if (!fromURLParam)
      onServerError(e);
    return;
  }
  Server.server.addEventListener('open', onServerOpen);
  Server.server.addEventListener('message', onServerMessage);
  Server.server.addEventListener('error', onServerError);
  Server.server.addEventListener('close', onServerClose);
}

function disconnectFromServer() {
  if (Server.server == undefined || Server.server.readyState != WebSocket.OPEN)
    return;
  Server.server.close();
}

function onServerOpen(e) {
  if (Server.server == undefined || Server.server.readyState != WebSocket.OPEN)
    return; // just in case it instantly closed

  //  l("connection-info").classList.remove("error");
  //  l("connection-info").classList.add("success");
  //  l("connection-info").innerText = "Connection successful";

  l("connect-button").innerText = "Disconnect";
  l("connect-button").onclick = disconnectFromServer;

  Server.server.send(JSON.stringify({
    req: "version",
    client_version
  }));
}

var minimum_server_version = {
  name: "standard compliant",
  major: 1,
  minor: 1,
  patch: 0
};

function verify_compatible_version(v) {
  let mv = minimum_server_version;
  if (!v.name.endsWith(mv.name))
    return false;

  if (v.major < mv.major)
    return false;
  if (v.major > mv.major)
    return true;

  if (v.minor < mv.minor)
    return false;
  if (v.minor > mv.minor)
    return true;

  if (v.patch < mv.patch)
    return false;
  return true;

}

function giveUpWaitingForOpponent() {
  Server.server.send(JSON.stringify({
    req: "close_game"
  }));
  hideWindows();
  showWindow("game-explorer");
  Server.requested_close = true;
}

function createOnlineGame() {
  if (Server.in_online_game && !confirm("This will close the online game you're currently in. Continue anyway?"))
    return;

  let sets = prelimSettings.online;
  if (sets == undefined)
    sets = {};

  let as_white = sets.playAsWhite;
  if (as_white == undefined)
    as_white = true;

  let private = sets.priv;
  if (private == undefined)
    private = false;

  let useTime = sets.useTime;
  if (useTime == undefined)
    useTime = true;

  let mainTimeMin = 5;
  let mainTimeSec = 0;
  let inct = "NO_CLOCK";
  let increment = 0;
  if (useTime) {
    let _mainTimeMin = parseInt(sets.mainTimeMin);
    if (_mainTimeMin == undefined || isNaN(_mainTimeMin))
      _mainTimeMin = 5;
    mainTimeMin = _mainTimeMin;

    let _mainTimeSec = parseInt(sets.mainTimeSec);
    if (_mainTimeSec == undefined || isNaN(_mainTimeSec))
      _mainTimeSec = 0;
    mainTimeSec = _mainTimeSec;

    let _inct = sets.incrementMethod;
    if (_inct == undefined)
      _inct = "BRONSTEIN";
    inct = _inct;

    let _increment = parseInt(sets.incrementAmount);
    if (_increment == undefined || isNaN(_increment))
      _increment = 15000;
    increment = _increment;
  }
  let starting_time = (mainTimeMin * 60 + mainTimeSec) * 1000;

  let msg = JSON.stringify({
    req: "create_game",
    private,
    as_white,
    starting_time,
    increment,
    inct
  })
  console.log(msg)
  Server.server.send(msg);
}

function joinOnlineFromList(e) {
  let id = e.srcElement.dataset.gamecode;
  Server.server.send(JSON.stringify({
    req: "join_game",
    id
  }));
}

function joinOnlineFromID() {
  let id = l("ge-game-code").value.toUpperCase();
  Server.server.send(JSON.stringify({
    req: "join_game",
    id
  }));
}

function onServerMessage(e) {
  console.log(e)
  try {
    let response = JSON.parse(e.data);
    let res = response.res;

    if (res == "error")
      onServerError(response.error)
    else if (res == "conn_success") {
      l("connection-info").classList.remove("error");
      l("connection-info").classList.add("success");
      l("connection-info").innerText = "Successfully logged in as " + response.name;
      l("username").value = response.name;
      l("name-box").style.display = "";

      updateGameList();

      Server.connection_verified = true;
      l("menu-open-ge").classList.remove("button-disable");

      l("offline-go-online").disabled = false;
    }
    else if (res == "version") {
      let serve_compat = verify_compatible_version(response.version);
      if (!serve_compat) {
        onServerError("Incompatible version");
        Server.server.close();
      }
    }
    else if (res == "user_set") {
      l("connection-info").classList.remove("error");
      l("connection-info").classList.add("success");
      l("connection-info").innerText = "Successfully set username to " + response.name;
    }
    else if (res == "game_list") {
      let games = response.list;
      console.log(games)
      // sort the games from first-created to most recently created
      games.sort((a, b) => (a.creation_time > b.creation_time) ? 1 : -1);

      gameList = l("game-list");
      gameList.innerHTML = "";

      if (games.length == 0) {
        let p = document.createElement("p");
        p.innerText = "No games public games right now.\nCreate one to get the party started!";
        gameList.appendChild(p);
        return;
      }

      for (let g of games) {
        try {
          let div = document.createElement("div");
          div.classList.add("game-list-entry");

          let playersBox = document.createElement("div");

          let whitePlayerBox = document.createElement("div");
          let blackPlayerBox = document.createElement("div");

          whitePlayerBox.classList.add("game-list-player-box");
          blackPlayerBox.classList.add("game-list-player-box");

          let whitePawn = document.createElement("img");
          whitePawn.src = "img/pieces/wp.svg";
          let blackPawn = document.createElement("img");
          blackPawn.src = "img/pieces/bp.svg";

          whitePlayerBox.appendChild(whitePawn);
          blackPlayerBox.appendChild(blackPawn);

          let whiteName = document.createElement("p");
          let blackName = document.createElement("p");

          if (g.join_as_white) {
            whiteName.innerText = "You?";
            blackName.innerText = g.host_name;
          }
          else {
            whiteName.innerText = g.host_name;
            blackName.innerText = "You?";
          }

          whitePlayerBox.appendChild(whiteName);
          blackPlayerBox.appendChild(blackName);

          playersBox.appendChild(whitePlayerBox);
          playersBox.appendChild(blackPlayerBox);

          div.appendChild(playersBox);

          let clockBox = document.createElement("div");
          clockBox.classList.add("game-list-clock-box");
          let clock = document.createElement("img");
          clock.src = "img/clock.png";
          clock.style.height = "1em";
          clockBox.appendChild(clock);

          // TODO #34: don't display this info if the game is untimed
          let startingTime = g.starting_time;

          let seconds = Math.floor(startingTime / 1000);
          let minutes = Math.floor(seconds / 60);
          seconds %= 60;
          let hours = Math.floor(minutes / 60);
          minutes %= 60;

          let timeText = (minutes + "").padStart(2, "0") + ":" + (seconds + "").padStart(2, "0");
          if (hours != 0)
            timeText = hours + ":" + timeText;

          let increment = g.increment;

          seconds = Math.floor(increment / 1000);
          minutes = Math.floor(seconds / 60);
          seconds %= 60;
          hours = Math.floor(minutes / 60);
          minutes %= 60;

          let incTimeText = (seconds + "").padStart(2, "0");
          if (minutes != 0 || hours != 0)
            incTimeText = (minutes + "").padStart(2, "0") + ":" + incTimeText;
          if (hours != 0)
            incTimeText = hours + ":" + incTimeText;

          let timeBox = document.createElement("p");
          // TODO #34: choose a better way to show the increment type
          timeBox.innerText = " " + timeText + " | " + incTimeText + " " + g.inct.toLowerCase();

          clockBox.appendChild(timeBox);

          div.appendChild(clockBox);

          let joinButton = document.createElement("button");
          joinButton.dataset.gamecode = g.id;
          joinButton.innerText = "Join";
          joinButton.onclick = joinOnlineFromList;
          div.appendChild(joinButton);

          gameList.appendChild(div);
        }
        catch (e) {
          // don't let a bad response get you down! just ignore it and continue
          console.error(e);
        }
      }
    }
    else if (res == "game_start") {
      Server.in_online_game = true;
      l("resign_button").style.display = "";
      l("show_results_button").style.display = "none";
      resetAftergameOptions();
      l("online-aftergame").style.display = "";
      l("rematch-button").innerText = "Request rematch";
      l("rematch-button").disabled = false;
      Server.play_as = response.play_as_white ? 'w' : 'b';
      hideWindows();
      Server.server.send(JSON.stringify({
        req: "get_board"
      }));

      Server.requested_close = false;

      l("aftergame-info").innerText = "";

      // TODO proably a lot more tbh
    }
    else if (res == "game_closed") {
      Server.in_online_game = false;
      l("resign_button").style.display = "none";

      Server.play_as = 'x';

      if (!Server.requested_close) {
        l("aftergame-info").innerText = "Opponent disconnected";
        l("aftergame-info").classList.remove("rematch");
        l("aftergame-info").classList.add("disconnect");

        l("show_results_button").style.display = "";
        resetAftergameOptions();
        l("online-aftergame").style.display = "";

        l("rematch-button").disabled = true;

        showWindow("result-screen");

        Game.game.clock.delete();
        Game.game.clock = new Module.Clock(1000, 0, Module.IncrementMethod.NO_CLOCK);
      }
      // TODO probably a lot more tbh
    }
    else if (res == "game_created") {
      let id = response.id;
      hideWindows();
      l("join-code").innerText = id;
      let serverJoinCode = Server.url_base + "/" + id;
      l("server-join-code").innerText = serverJoinCode;
      l("url-join-code").innerText = location.origin + location.pathname + "?" + serverJoinCode;
      showWindow("waiting-for-opponent");
    }
    else if (res == "board_state") {
      console.log(response);

      Game.game.board.delete();
      Game.game.board = new Module.Board(response.board_str);

      Game.game.clock.delete();
      Game.game.clock = new Module.Clock(
        response.white_timer.time_left,
        response.white_timer.increment,
        Module.IncrementMethod[response.white_timer.inct],
        response.black_timer.time_left,
        response.black_timer.increment,
        Module.IncrementMethod[response.black_timer.inct]
      );

      Game.game.startClock(Game.game.board.turn);

      if (!selectedSquares)
        selectedSquares = {};
      if (!selectedSquares.prev_move)
        selectedSquares.prev_move = {};
      if (selectedSquares.prev_move.from)
        selectedSquares.prev_move.from.delete();
      if (selectedSquares.prev_move.to)
        selectedSquares.prev_move.to.delete();
      selectedSquares.prev_move.from = new Module.Square(response.prev_from);
      selectedSquares.prev_move.to = new Module.Square(response.prev_to);

      dispboard(Game.game.board);
    }
    else if (res == "opponent_resigned") {
      l("winner").innerText = "Opponent resigned";
      l("winner-info").innerText = "";
      showWindow("result-screen");
      l("show_results_button").style.display = "";
      l("resign_button").style.display = "none";

    }
    else if (res == "you_resigned") {
      l("winner").innerText = "You resigned";
      l("winner-info").innerText = "";
      showWindow("result-screen");
      l("show_results_button").style.display = "";
      l("resign_button").style.display = "none";
    }
    else if (res == "rematch_reqd") {
      if (!you_requested_the_rematch) {
        l("aftergame-info").innerText = "Opponent requested rematch";
        l("aftergame-info").classList.add("rematch");
        l("aftergame-info").classList.remove("disconnect");
        l("rematch-button").innerText = "Start rematch";
      }

    }
  }
  catch (error) {
    onServerError(error);
  }
  // don't add code after here, sections might return without reaching it
}

function onServerError(e) {
  let text = e;
  if (e.message)
    text = e.message;
  else if (typeof e == "object")
    text = "Failed to connect to server";
  l("connection-info").innerText = text;
  l("connection-info").classList.remove("success");
  l("connection-info").classList.add("error");

  if (l("manage-connection").style.display == "")
    alert("Error:"+text);
}

function onServerClose(e) {
  console.log(e)
  l("connection-info").innerText += "\nConnection closed";
  if (e.reason)
    l("connection-info").innerText += ": " + e.reason;
  if (e.code == 1000)
    l("connection-info").classList.remove("error");
  else
    l("connection-info").classList.add("error");
  l("connection-info").classList.remove("success");
  //server = undefined;
  l("connect-button").innerText = "Connect";
  l("connect-button").onclick = () => connectToServer();
  l("name-box").style.display = "none";

  if (l("manage-connection").style.display == "") {
    alert("Connection to server terminated");
    hideWindows();
    showWindow('manage-connection');
  }

  Server.connection_verified = false;
  Server.in_online_game = false;
  l("resign_button").style.display = "none";
  resetAftergameOptions();
  l("offline-aftergame").style.display = "";
  l("menu-open-ge").classList.add("button-disable");
  l("offline-go-online").disabled = false;
}

function set_username() {
  let name = l("username").value;
  if (!name)
    return;
  Server.server.send(JSON.stringify({
    req: "user_set",
    name
  }));
}

function updateGameList() {
  try {
    Server.server.send(JSON.stringify({
      req: "list_games"
    }));
  }
  catch (e) {
    l('game-list').innerHTML = "";
    let p = document.createElement("p");
    p.innerText = "Could not connect to server...";
    l('game-list').appendChild(p);
  }

  // response will be handled in onServerMessage
}

function pieceTypeToString(pt) {
  if (pt == Module.PieceType.PAWN)
    return "PAWN";
  if (pt == Module.PieceType.ROOK)
    return "ROOK";
  if (pt == Module.PieceType.KNIGHT)
    return "KNIGHT";
  if (pt == Module.PieceType.BISHOP)
    return "BISHOP";
  if (pt == Module.PieceType.QUEEN)
    return "QUEEN";
  if (pt == Module.PieceType.KING)
    return "KING";
  return "NO";
}

function sendMove(move) {
  let from = move.from.toIndex();
  let to = move.to.toIndex();
  let promo = pieceTypeToString(move.promotion);

  Server.server.send(JSON.stringify({
    req: "make_move",
    from,
    to,
    promo
  }));
}

function resign() {
  if (confirm("Really resign this game?")) {
    Server.server.send(JSON.stringify({
      "req": "resign"
    }));
  }
}

var you_requested_the_rematch = false;
function requestRematch() {
  you_requested_the_rematch = true;
  l("rematch-button").innerText = "Waiting for opponent...";
  Server.server.send(JSON.stringify({
    req: "request_rematch"
  }));
}

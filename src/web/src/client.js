var server = undefined;
var connection_verified = false;
var in_online_game = false;

function connectToServer()
{
  let ip = l("server-ip").value;
  try
  {
    server.close();
  } catch(e){}
  try
  {
    server = new WebSocket("ws://"+ip);
  }
  catch(e)
  {
    onServerError(e);
    return;
  }
  server.addEventListener('open', onServerOpen);
  server.addEventListener('message', onServerMessage);
  server.addEventListener('error', onServerError);
  server.addEventListener('close', onServerClose);
}

function disconnectFromServer()
{
  if(server == undefined || server.readyState != WebSocket.OPEN)
    return;
  server.close();
}

function onServerOpen(e)
{
  if(server == undefined || server.readyState != WebSocket.OPEN)
    return; // just in case it instantly closed
    
//  l("connection-info").classList.remove("error");
//  l("connection-info").classList.add("success");
//  l("connection-info").innerText = "Connection successful";
  
  l("connect-button").innerText = "Disconnect";
  l("connect-button").onclick = disconnectFromServer;
  
  server.send(JSON.stringify({
    req:"version",
    client_version
  }));
}

var minimum_server_version = {
  name: "standard compliant",
  major: 0,
  minor: 0,
  patch: 0
};

function verify_compatible_version(v)
{
  let mv = minimum_server_version;
  if(!v.name.endsWith(mv.name))
    return false;
      
  if(v.major < mv.major)
    return false;
  if(v.major > mv.major)
    return true;
    
  if(v.minor < mv.minor)
    return false;
  if(v.minor > mv.minor)
    return true;
    
  if(v.patch < mv.patch)
    return false;
  return true;
  
}


function joinOnlineFromList(e)
{
  let id = e.srcElement.dataset.gamecode;
  server.send(JSON.stringify({
    req: "join_game",
    id
  }));
}

function onServerMessage(e)
{
  console.log(e)
  try
  {
    let response = JSON.parse(e.data);
    let res = response.res;
    
    if(res == "error")
      onServerError(response.error)
    else if(res == "conn_success")
    {
      l("connection-info").classList.remove("error");
      l("connection-info").classList.add("success");
      l("connection-info").innerText = "Successfully logged in as " + response.name;
      l("username").value = response.name;
      l("name-box").style.display = "";

      updateGameList();

      connection_verified = true;
      l("menu-open-ge").classList.remove("button-disable");
    }
    else if(res == "version")
    {
      let serve_compat = verify_compatible_version(response.version);
      if(!serve_compat)
      {
        onServerError("Incompatible version");
        server.close();
      }
    }
    else if(res == "user_set")
    {
      l("connection-info").classList.remove("error");
      l("connection-info").classList.add("success");
      l("connection-info").innerText = "Successfully set username to " + response.name;
    }
    else if(res == "game_list")
    {
      let games = response.list;
      console.log(games)
      // sort the games from first-created to most recently created
      games.sort((a,b) => (a.creation_time > b.creation_time) ? 1:-1);
      
      gameList = l("game-list");
      gameList.innerHTML = "";

      if(games.length == 0)
      {
        let p = document.createElement("p");
        p.innerText = "No games public games right now.\nCreate one to get the party started!";
        gameList.appendChild(p);
        return;
      }

      for(let g of games)
      {
        try
        {
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

          if(g.join_as_white)
          {
            whiteName.innerText = "You?";
            blackName.innerText = g.host_name;
          }
          else
          {
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

          let startingTime = g.starting_time;

          let seconds = Math.floor(startingTime/1000);
          let minutes = Math.floor(seconds / 60);
          seconds %= 60;
          let hours = Math.floor(minutes / 60);
          minutes %= 60;
          
          let timeText = (minutes+"").padStart(2, "0") + ":" + (seconds+"").padStart(2, "0");
          if(hours != 0)
            timeText = hours + ":" + timeText;

          let increment = g.increment;

          seconds = Math.floor(increment/1000);
          minutes = Math.floor(seconds / 60);
          seconds %= 60;
          hours = Math.floor(minutes / 60);
          minutes %= 60;
          
          let incTimeText = (seconds+"").padStart(2, "0");
          if(minutes != 0 || hours != 0)
            incTimeText = (minutes+"").padStart(2, "0") + ":" + incTimeText;
          if(hours != 0)
            incTimeText = hours + ":" + incTimeText;

          let timeBox = document.createElement("p");
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
        catch(e)
        {
          // don't let a bad response get you down! just ignore it and continue
          console.error(e);
        }
      }
    }
    else if(res == "game_start")
    {
      in_online_game = true;
      hideWindows();
      // TODO proably a lot more tbh
    }
    else if(res == "game_closed")
    {
      in_online_game = false;
    }

    // TODO add game_created, game_closed
    
  }
  catch(error)
  {
    onServerError(error);
  }
  // don't add code after here, sections might return without reaching it
}

function onServerError(e)
{
  let text = e;
  if(e.message)
    text = e.message;
  else if(typeof e == "object")
    text = "Failed to connect to server";
  l("connection-info").innerText = text;
  l("connection-info").classList.remove("success");
  l("connection-info").classList.add("error");
  
  if(l("manage-connection").style.display == "")
    alert(text);
}

function onServerClose(e)
{
  console.log(e)
  l("connection-info").innerText += "\nConnection closed";
  if(e.reason)
    l("connection-info").innerText += ": "+e.reason;
  if(e.code == 1000)
    l("connection-info").classList.remove("error");
  else
    l("connection-info").classList.add("error");
  l("connection-info").classList.remove("success");
  //server = undefined;
  l("connect-button").innerText = "Connect";
  l("connect-button").onclick = connectToServer;
  l("name-box").style.display = "none";

  if(l("manage-connection").style.display == "")
  {
    alert("Connection to server terminated");
    hideWindows();
    showWindow('manage-connection');
  }

  connection_verified = false;
  in_online_game = false;
  l("menu-open-ge").classList.add("button-disable");
}

function set_username()
{
  let name = l("username").value;
  if(!name)
    return;
  server.send(JSON.stringify({
    req:"user_set",
    name
  }));
}

function updateGameList()
{
  try
  {
    server.send(JSON.stringify({
      req: "list_games"
    }));
  }
  catch(e)
  {
    l('game-list').innerHTML = "";
    let p = document.createElement("p");
    p.innerText = "Could not connect to server...";
    l('game-list').appendChild(p);
  }

  // response will be handled in onServerMessage
}
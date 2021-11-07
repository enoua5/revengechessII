////////////////////////////////////////////////////////////////////////////////
function numCoordToString(x, y)
{
  return String.fromCharCode(97+x)+""+(y+1);
}
function indexToStringCoord(i)
{
  let x = i%8;
  let y = Math.floor(i/8);
  return numCoordToString(x,y)
}
function stringToSquare(s)
{
  if(s.split("square_").length > 1)
  {
    s = s.split("square_")[1];
  }
  s = s.toLowerCase();
  let f = s.charCodeAt(0)-97;
  let r = s.charCodeAt(1)-49;
  return new Module.Square(f,r);
}
function squareToBoard(s)
{
  return l("square_"+(s.toString().toLowerCase()));
}

////////////////////////////////////////////////////////////////////////////////

function showResultScreen()
{
  let clock = Game.game.clock;
  let board = Game.game.board;

  let winnerText = "ERROR";
  
  let winner = clock.getResultFromFlag();
  let byTime = false;
  if(winner != Module.GameResult.ONGOING)
  {
    l("winner-info").innerText = "By time";
    byTime = true;
  }
  else
    winner = board.getGameResult(true);
  
  switch(winner)
  {
    case Module.GameResult.WHITE_VICTORY:
      winnerText = "White wins!";
      break;
    case Module.GameResult.BLACK_VICTORY:
      winnerText = "Black wins!";
      break;
    case Module.GameResult.STALEMATE:
      winnerText = "It's a draw.";
      break;
  }
  l("winner").innerText = winnerText;
  
  if(!byTime && winner != Module.GameResult.STALEMATE)
    l("winner-info").innerText = "By checkmate";
  else if(!byTime)
    l("winner-info").innerText = "";
    
  showWindow("result-screen");
  l("show_results_button").style.display = "";
  
  board.delete();
  clock.delete();
}

////////////////////////////////////////////////////////////////////////////////

function whiteCurrentlyOnTop()
{
  return l("board").firstChild.id=="square_h1";
}
function dispboard(board)
{
  if(board.getGameResult(true) != Module.GameResult.ONGOING)
  {
    showResultScreen();
  }

  let whiteOnTop = shouldWhiteBeOnTop();
  
  for(let i = 0; i < 64; i++)
  {
    let s = l("square_"+indexToStringCoord(i));
    s.innerHTML = "";
    
    let p = board.getPlayField(i);
    
    if(p == Module.PieceIdentifier.EMPTY)
      continue;
    
    
    let typeAfterPromotion = board.getBoardPiece(p.value).type;
    s.appendChild(createPieceImage(Module.colorOfPiece(p), typeAfterPromotion));
  }
  
  
  if(whiteCurrentlyOnTop() != whiteOnTop)
  {
    let board_dom = l("board");
    let children = board_dom.childNodes;
    let num = children.length;
    for(let i = num-1; i >= 0; i--)
    {
      let c = board_dom.removeChild(children[i]);
      board_dom.appendChild(c);
    }
  }
  
  dispPrevMove();
  clearPieceInfo();
  squareHovered(selectedSquares.hover); // rehover the square now that things have moved around
  
  // start the AI if it needs to be started
  if(getCurrentPlayerSets().usingAI)
    makeAIMove();
    
}

function dispValidDestinations()
{
  let moves = [];
  if(selectedSquares.from != null)
  {
    let board = Game.game.board;
    let moves_vector = board.getValidMoves();
    board.delete();
    for(let i = 0; i < moves_vector.size(); i++)
    {
      let move = moves_vector.get(i);
      // only add it to the list if it's from the selected square
      if(move.from.eq(selectedSquares.from))
      {
        moves.push(move);
      }
    }
    moves_vector.delete();
  }
  
  for(let i of l("board").children)
  {
    let valid_dest = false;
    for(let m of moves)
    {
      // if the square we're looking at is the destination square
      if(stringToSquare(i.id).eq(m.to))
      {
        i.classList.add("possible_move");
        valid_dest = true;
        break;
      }
    }
    if(!valid_dest)
    {
      i.classList.remove("possible_move");
    }
  }
}

function clearPrevMove()
{
  let HTML_list = document.querySelectorAll(".square.prev_move");
  // document.get[whatever]() returns an HTMLCollection
  // HTMLCollections have weird behaviour when itterating and modifying
  // Namely, they are removed from the collection if their selector no longer matches
  // So, we're moving it to a list
  let list = [];
  for(let i of HTML_list)
  {
    list.push(i);
  }
  
  for(let i in list)
  {
    list[i].classList.remove("prev_move")
  }
}

function dispPrevMove()
{
  clearPrevMove();
  
  if(selectedSquares.prev_move == null || Object.keys(selectedSquares.prev_move).length == 0)
    return;
    
  
  // not really needed, but don't trust that the squares you're getting are valid
  try
  {
    let fromSquare = squareToBoard(selectedSquares.prev_move.from);
    let toSquare = squareToBoard(selectedSquares.prev_move.to);
    fromSquare.classList.add("prev_move");
    toSquare.classList.add("prev_move");
  }
  catch(e)
  {
    console.trace(e);
    console.error("Something went wrong with displaying the previous move.");
    console.warn("If this was not triggered intentially, there is probably a much larger problem at play")
  }
  
}

////////////////////////////////////////////////////////////////////////////////

var selectedSquares = {
  from: null,
  to: null,
  hover: null,
  prev_move: null,
  awaiting_promo: false
}

function destroyPromotionSelectBoxes()
{
  let boxes = document.querySelectorAll(".promotion_selector");
  if(boxes.length > 1) console.warn(boxes.length + " promotion selectors were on the board at the same time. Look into that.");
  
  for(let i of boxes)
  {
    i.remove();
  }
}

function cancelPromotion()
{
  selectedSquares.awaiting_promo = false;
  selectedSquares.from = null;
  selectedSquares.to = null;
  destroyPromotionSelectBoxes();
  dispValidDestinations();
}

function createPromotionSelectBox(flip, side)
{
  selectedSquares.awaiting_promo = true;
  if(flip == undefined)
    flip = false;
  if(side == undefined)
    side = false;

  let select_box = document.createElement("div");
  select_box.classList.add("promotion_selector");
  let selections = document.createElement("div");
  select_box.appendChild(selections);
  let cancel_button = document.createElement("button");
  cancel_button.innerText = "X"; // TODO replace with image
  select_box.appendChild(cancel_button);
  cancel_button.onclick = cancelPromotion; // do not call, we're linking the actual funciton pointer
  
  let board = Game.game.board;
  let turn = board.turn;
  board.delete();
  let king = createPieceImage(turn, Module.PieceType.KING);
  king.onclick = ()=>selectPromotion('k');
  selections.appendChild(king);
  let queen = createPieceImage(turn, Module.PieceType.QUEEN);
  queen.onclick = ()=>selectPromotion('q');
  selections.appendChild(queen);
  let rook = createPieceImage(turn, Module.PieceType.ROOK);
  rook.onclick = ()=>selectPromotion('r');
  selections.appendChild(rook);
  let bishop = createPieceImage(turn, Module.PieceType.BISHOP);
  bishop.onclick = ()=>selectPromotion('b');
  selections.appendChild(bishop);
  let knight = createPieceImage(turn, Module.PieceType.KNIGHT);
  knight.onclick = ()=>selectPromotion('n');
  selections.appendChild(knight);
  let pawn = createPieceImage(turn, Module.PieceType.PAWN);
  pawn.onclick = ()=>selectPromotion('p');
  selections.appendChild(pawn);
  
  if(flip)
    selections.style.top = "-200%";
  if(side == "left")
    selections.style.left = "0%";
  if(side == "right")
    selections.style.left = "-200%"
  
  return select_box;
}

function createPieceImage(color, type)
{
  // piece together the url of the piece imgage bit by bit
  // TODO look up table would probably be better
  let src = "img/pieces/";
  src += (color == Module.PlayerColor.BLACK) ? "b":"w";
  switch(type)
  {
    case Module.PieceType.PAWN:
      src+="p";
    break;
    case Module.PieceType.ROOK:
      src+="r";
    break;
    case Module.PieceType.KNIGHT:
      src+="n";
    break;
    case Module.PieceType.BISHOP:
      src+="b";
    break;
    case Module.PieceType.QUEEN:
      src+="q";
    break;
    case Module.PieceType.KING:
      src+="k";
    break;
  }
  src+=".svg";
  
  let img = document.createElement("img");
  img.src = src;
  return img;
}

function selectPromotion(p)
{
  let promo = Module.PieceType.NO;
  switch(p)
  {
    case 'p':
      promo = Module.PieceType.PAWN;
    break;
    case 'r':
      promo = Module.PieceType.ROOK;
    
    break;
    case 'n':
      promo = Module.PieceType.KNIGHT;
    
    break;
    case 'b':
      promo = Module.PieceType.BISHOP;
    
    break;
    case 'q':
      promo = Module.PieceType.QUEEN;
    
    break;
    case 'k':
      promo = Module.PieceType.KING;
    
    break;
    default:
      cancelPromotion();
  }
  
  
  let move = new Module.Move(selectedSquares.from, selectedSquares.to, promo);
  selectedSquares.prev_move = {
    from: selectedSquares.from,
    to: selectedSquares.to,
    promotion: selectedSquares.promotion
  };
  makeMove(move);
  let board = Game.game.board;
  dispboard(board);
  board.delete();
  cancelPromotion(); // perhaps a bad name. Just clears the selection box
}

function dispRespawnSquares(respawnSquares)
{
  for(let i of l("board").children)
  {
    if(respawnSquares.indexOf(i.id) != -1)
      i.classList.add("respawn_point");
    else
      i.classList.remove("respawn_point");
  }
}

function squareHovered(e, which)
{
  if(which == null)
  {
    dispRespawnSquares([]);
    return;
  }
  selectedSquares.hover = which;
  let s = stringToSquare(which.id);
  let board = Game.game.board;
  let pid = board.getPlayField(s.toIndex());
  if(pid == Module.PieceIdentifier.EMPTY)
  {
    dispRespawnSquares([]);
    board.delete();
    return;
  }
  let p = board.getBoardPiece(pid.value);
  
  
  
  let captures = [];
  for(let i = 0; i < p.numCaptures; i++)
    captures.push(p.getCapture(i));
  
  let respawnSquares = [];
  for(let cap of captures)
  {
    let piece = board.getBoardPiece(cap.value);
    let home = piece.home;
    respawnSquares.push("square_"+numCoordToString(home.file, home.rank));
  }
  
  showPieceInfo(pid, captures);
  dispRespawnSquares(respawnSquares);
  board.delete();
}

function squareUnhovered(e, which)
{
  clearPieceInfo();
  if(which == selectedSquares.hover)
  {
    selectedSquares.hover = null;
    dispRespawnSquares([]);
  }
}

function droppedOutside()
{
  if(selectedSquares.from == null || selectedSquares.awaiting_promo)
    return;
  squareToBoard(selectedSquares.from).firstChild.style.opacity = "1";
  squareToBoard(selectedSquares.from).classList.remove("selected");
  selectedSquares.from = null;
  selectedSquares.to = null;
  dispValidDestinations();
}

function squareClicked(e, which)
{
  if(e.type == "drop")
    e.preventDefault()
  
  // do not allow the player to move on the AI's turn
  if(getCurrentPlayerSets().usingAI)
    return;
  if(gameIsOver())
    return;
  if(Server.in_online_game)
  {
    let turn = Game.game.board.turn == Module.PlayerColor.WHITE ? 'w' : 'b';
    if(turn != Server.play_as)
      return;
  }
  
  let s = stringToSquare(which.id);
  
  let board = Game.game.board;
  let moves = board.getValidMoves();
  // if we're selecting the from square
  if(selectedSquares.from == null)
  {
    let validFromSquare = false;
    for(let i = 0; i < moves.size(); i++)
    {
      let m = moves.get(i);
      let posFrom = m.from;
      m.delete();
      
      if(s.eq(posFrom))
      {
        validFromSquare = true;
        posFrom.delete();
        break;
      }
      posFrom.delete();
    }
    
    if(validFromSquare)
    {
      if(e.type == "dragstart")
      {
        //using this as opposed to display: none, visibility: hidden, or hidden = true, because Chrome disables the drag events for hidden elements.
        which.firstChild.style.opacity = "0";
      }
      
      
      selectedSquares.from = s;
      // display selected indicator
      which.classList.add("selected");
      dispValidDestinations();
    }
    else
    {
      selectedSquares.from = null;
      dispValidDestinations();
    }
  }
  
  //if we're selecting the destination square
  else
  {
    if(e.type == "drop")
    {
      squareToBoard(selectedSquares.from).firstChild.style.opacity = "1"
    }
  
    // remove the selected piece indicator
    let sel = document.querySelector(".square.selected");
    if(sel)
      sel.classList.remove("selected");
  
    let validToSquare = false;
    for(let i =0; i < moves.size(); i++)
    {
      let posMove = moves.get(i);
      let posFrom = posMove.from;
      let posTo = posMove.to;
      posMove.delete();
      // if the move is from the previously selected from square
      if(posFrom.eq(selectedSquares.from))
      {
        if(posTo.eq(s))
        {
          validToSquare = true;
          posFrom.delete();
          posTo.delete();
          break;
        }
      }
      posFrom.delete();
      posTo.delete();
    }
    
    if(validToSquare)
    {
      // if the moving peice is a pawn ...
      let pieceID = board.getPlayField(selectedSquares.from.toIndex()).value;
      let trueType = board.getBoardPiece(pieceID).type
      
      if(trueType == Module.PieceType.PAWN)
      {
        // and is ending up on the last file ...
        if(s.rank == 0 || s.rank == 7)
        {
          // then the move isn't complete. A promotion must be selected!
          selectedSquares.to = s;
          
          // we have to find out if it's on the bottom or on a side
          let index = indexOfElement(which);
          let on_bottom = index >= 56;
          let file = index % 8;
          let side = false;
          if(file == 0)
            side = "left"
          if(file == 7)
            side = "right"
          
          let select_box = createPromotionSelectBox(on_bottom, side);
          which.appendChild(select_box);
          board.delete();
          moves.delete();
          return;
        }
      }
      
      let move = new Module.Move(selectedSquares.from, s);
      selectedSquares.prev_move = {
        from: move.from,
        to: move.to,
        promotion: move.promotion
      };
      makeMove(move);
      move.delete();
      board.delete();
      board = Game.game.board;
      dispboard(board);
    }
    
    selectedSquares.from = null;
    selectedSquares.to = null;
    // clear indicators
    dispValidDestinations();
  }
  board.delete();
  moves.delete();
}

////////////////////////////////////////////////////////////////////////////////

function copyAISettings(src, dest)
{
  let def = defaultEngineSettings();

    dest.usingAI = (src.usingAI == undefined) ? def.usingAI : src.usingAI;
    
    dest.limitMode = (src.limitMode == undefined) ? def.limitMode : src.limitMode;
    if(dest.limitMode == "time")
      dest.limitMode = SearchLimits.CONSTANT_TIME;
    else if(dest.limitMode == "depth")
      dest.limitMode = SearchLimits.CONSTANT_DEPTH;
    else if(dest.limitMode == "auto")
      dest.limitMode = SearchLimits.AUTOMATIC;
      
    dest.maxTime = (src.maxTime == undefined) ? def.maxTime : parseInt(src.maxTime)*1000;
    
    dest.limitSeconds = (src.limitSeconds == undefined) ? def.limitSeconds : parseInt(src.limitSeconds)*1000;
    
    dest.limitPlys = (src.limitPlys == undefined) ? def.limitPlys : parseInt(src.limitPlys);
    
    dest.flagBuffer = (src.flagBuffer == undefined) ? def.flagBuffer : parseInt(src.flagBuffer);
    
    dest.minDelta = (src.minDelta == undefined) ? def.minDelta : parseInt(src.minDelta);
    
    dest.maxDelta = (src.minDelta == undefined) ? def.maxDelta : parseInt(src.maxDelta);
}

function newGame()
{
  if(Server.in_online_game)
  {
    if(!confirm("This will close the online game you're currently in. Continue anyway?"))
      return;
    Server.server.send(JSON.stringify({
      req: "close_game"
    }));
    Server.requested_close = true;
  }
  else
  {
    l("resign_button").style.display = "none";
    l("offline-aftergame").style.display = "";
    l("online-aftergame").style.display = "none";
  }

  alreadyShowedResultScreen = false;
  l("show_results_button").style.display = "none";
  
  if(!prelimSettings.white)
    prelimSettings.white = {};
  if(!prelimSettings.black)
    prelimSettings.black = {};
  copyAISettings(prelimSettings.white, Settings.ai.white);
  copyAISettings(prelimSettings.black, Settings.ai.black);
  
  if(Game.game)
    Game.game.delete();
  Game.game = new Module.Game();
  
  // TODO less hardcoded values please
  let wIncAmt = prelimSettings.white.incrementAmount;
  if(wIncAmt == undefined)
    wIncAmt = 15;
  wIncAmt = parseInt(wIncAmt)*1000;
  
  let wTimeMin = prelimSettings.white.mainTimeMin;
  if(wTimeMin == undefined)
    wTimeMin = 5;
  wTimeMin = parseInt(wTimeMin);
  
  let wTimeSec = prelimSettings.white.mainTimeSec;
  if(wTimeSec == undefined)
    wTimeSec = 0;
  wTimeSec = parseInt(wTimeSec);
  
  let wMainTime = (wTimeMin*60 + wTimeSec)*1000;
  
  let wIncType = prelimSettings.white.incrementMethod;
  if(wIncType == undefined)
    wIncType = "bronstein";
    
  if(wIncType == "bronstein")
    wIncType = Module.IncrementMethod.BRONSTEIN;
  else if(wIncType == "increment")
    wIncType = Module.IncrementMethod.INCREMENT;
  else if(wIncType == "delay")
    wIncType = Module.IncrementMethod.DELAY;
    
  if(prelimSettings.white.useTime == undefined)
    prelimSettings.white.useTime = true;
  if(!prelimSettings.white.useTime)
    wIncType = Module.IncrementMethod.NO_CLOCK;
  
  if(prelimSettings.black.useSameTime == undefined)
    prelimSettings.black.useSameTime = true;
  if(prelimSettings.black.useSameTime)
  {
    let clock = new Module.Clock(wMainTime, wIncAmt, wIncType);
    Game.game.clock = clock;
  }
  else
  {
    let bIncAmt = prelimSettings.black.incrementAmount;
    if(bIncAmt == undefined)
      bIncAmt = 15;
    bIncAmt = parseInt(bIncAmt)*1000;
    
    let bTimeMin = prelimSettings.black.mainTimeMin;
    if(bTimeMin == undefined)
      bTimeMin = 5;
    bTimeMin = parseInt(bTimeMin);
    
    let bTimeSec = prelimSettings.black.mainTimeSec;
    if(bTimeSec == undefined)
      bTimeSec = 0;
    bTimeSec = parseInt(bTimeSec);
    
    let bMainTime = (bTimeMin*60 + bTimeSec)*1000;
    
    let bIncType = prelimSettings.black.incrementMethod;
    if(bIncType == undefined)
      bIncType = "bronstein";
      
    if(bIncType == "bronstein")
      bIncType = Module.IncrementMethod.BRONSTEIN;
    else if(bIncType == "increment")
      bIncType = Module.IncrementMethod.INCREMENT;
    else if(bIncType == "delay")
      bIncType = Module.IncrementMethod.DELAY;
    
    if(prelimSettings.black.useTime == undefined)
      prelimSettings.black.useTime = true;
    if(!prelimSettings.black.useTime)
      bIncType = Module.IncrementMethod.NO_CLOCK;
      
    let clock = new Module.Clock(wMainTime, wIncAmt, wIncType,
                                 bMainTime, bIncAmt, bIncType);
    Game.game.clock = clock;
  }
  
  Game.game.startClock(Module.PlayerColor.WHITE);
  selectedSquares.prev_move = {};
  
  let board = Game.game.board;
  dispboard(Game.game.board);
  board.delete();
}

function makeMove(move)
{
  if(!Server.in_online_game)
    Game.game.commitMove(move);
  else
    sendMove(move);
}
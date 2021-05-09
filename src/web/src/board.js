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

////////////////////////////////////////////////////////////////////////////////

function whiteCurrentlyOnTop()
{
  return l("board").firstChild.id=="square_h1";
}
function dispboard(board)
{
  let whiteOnTop = Settings.whiteOnTop; // TODO not how this is gonna work...
  
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
  
  squareHovered(selectedSquares.hover); // rehover the square not that things have moved around
}

function dispValidDestinations()
{
  let moves = [];
  if(selectedSquares.from != null)
  {
    let moves_vector = Game.game.board.getValidMoves();
    for(let i = 0; i < moves_vector.size(); i++)
    {
      let move = moves_vector.get(i);
      // only add it to the list if it's from the selected square
      if(move.from.eq(selectedSquares.from))
      {
        moves.push(move);
      }
    }
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

////////////////////////////////////////////////////////////////////////////////

var selectedSquares = {
  from: null,
  to: null,
  hover: null
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
  selectedSquares.from = null;
  selectedSquares.to = null;
  destroyPromotionSelectBoxes();
  dispValidDestinations();
}

function createPromotionSelectBox(flip, side)
{
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
  
  
  let turn = Game.game.board.turn;
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
  Game.game.commitMove(move);
  dispboard(Game.game.board);
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

function squareHovered(which)
{
  if(which == null)
  {
    dispRespawnSquares([]);
    return;
  }
  selectedSquares.hover = which;
  let s = stringToSquare(which.id);
  let pid = Game.game.board.getPlayField(s.toIndex());
  if(pid == Module.PieceIdentifier.EMPTY)
  {
    dispRespawnSquares([]);
    return;
  }
  let p = Game.game.board.getBoardPiece(pid.value);
  
  
  
  let captures = [];
  for(let i = 0; i < p.numCaptures; i++)
    captures.push(p.getCapture(i));
  
  let respawnSquares = [];
  for(let cap of captures)
  {
    let piece = Game.game.board.getBoardPiece(cap.value);
    let home = piece.home;
    respawnSquares.push("square_"+numCoordToString(home.file, home.rank));
  }
  
  dispRespawnSquares(respawnSquares);
}

function squareUnhovered(which)
{
  if(which == selectedSquares.hover)
  {
    selectedSquares.hover = null;
    dispRespawnSquares([]);
  }
}

function squareClicked(which)
{
  let s = stringToSquare(which.id);
  
  let moves = Game.game.board.getValidMoves();
  // if we're selecting the from square
  if(selectedSquares.from == null)
  {
    let validFromSquare = false;
    for(let i = 0; i < moves.size(); i++)
    {
      let posFrom = moves.get(i).from;
      if(s.eq(posFrom))
      {
        validFromSquare = true;
        break;
      }
    }
    
    if(validFromSquare)
    {
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
    // remove the selected piece indicator
    let sel = document.querySelector(".square.selected");
    if(sel)
      sel.classList.remove("selected");
  
    let validToSquare = false;
    for(let i =0; i < moves.size(); i++)
    {
      let posMove = moves.get(i);
      // if the move is from the previously selected from square
      if(posMove.from.eq(selectedSquares.from))
      {
        if(posMove.to.eq(s))
        {
          validToSquare = true;
          break;
        }
      }
    }
    
    if(validToSquare)
    {
      // if the moving peice is a pawn ...
      let pieceID = Game.game.board.getPlayField(selectedSquares.from.toIndex()).value;
      let trueType = Game.game.board.getBoardPiece(pieceID).type
      
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
          return;
        }
      }
      
      let move = new Module.Move(selectedSquares.from, s);
      Game.game.commitMove(move);
      dispboard(Game.game.board);
    }
    
    selectedSquares.from = null;
    selectedSquares.to = null;
    // clear indicators
    dispValidDestinations();
  }
}

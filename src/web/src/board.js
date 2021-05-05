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
  let f = s.charCodeAt(0)-97;
  let r = s.charCodeAt(1)-49;
  return new Module.Square(f,r);
}

////////////////////////////////////////////////////////////////////////////////

function whiteCurrentlyOnTop()
{
  return l("board").firstChild.id=="square_a1";
}
function dispboard(board, whiteOnTop = false)
{
  for(let i = 0; i < 64; i++)
  {
    let s = l("square_"+indexToStringCoord(i));
    s.innerHTML = "";
    
    let p = Module.getPlayField(board, i);
    
    if(p == Module.PieceIdentifier.EMPTY)
      continue;
    
    // piece together the url of the piece imgage bit by bit
    // TODO look up table would probably be better
    let src = "img/pieces/";
    src += (Module.colorOfPiece(p) == Module.PlayerColor.BLACK) ? "b":"w";
    switch(Module.typeOfPiece(p))
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
    
    s.appendChild(img);
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
}

////////////////////////////////////////////////////////////////////////////////

var selectedSquares = {
  from: null,
  to: null
}

function squareClicked(which)
{
  let id = which.id.split("square_")[1];
  let s = stringToSquare(id);
  
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
    }
    else
      selectedSquares.from = null;
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
      let move = new Module.Move(selectedSquares.from, s);
      Game.game.commitMove(move);
      dispboard(Game.game.board);
    }
    
    selectedSquares.from = null;
    selectedSquares.to = null;
  }
}

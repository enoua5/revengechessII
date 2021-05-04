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

var selectedSquares = {
  from: null
}

function squareClicked(which)
{
  let id = which.id.split("square_")[1];
  let s = stringToSquare(id);
  
  let moves = Game.game.board.getValidMoves();
  
  if(selectedSquares.from == null)
  {
    
    for(let i = 0; i < moves.length; i++)
    {
      let posFrom = moves.get(i).from;
    }
    
    selectedSquares.from = s;
  }
}

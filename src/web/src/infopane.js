function getPieceName(pid)
{
  return ([
    "White Queenside Rook",
    "White Queenside Knight",
    "White Queenside Bishop",
    "White Queen",
    "White Kingside Rook",
    "White Kingside Knight",
    "White Kingside Bishop",
    "White King",
    "White A Pawn",
    "White B Pawn",
    "White C Pawn",
    "White D Pawn",
    "White E Pawn",
    "White F Pawn",
    "White G Pawn",
    "White H Pawn",
    
    "Black Queenside Rook",
    "Black Queenside Knight",
    "Black Queenside Bishop",
    "Black Queen",
    "Black Kingside Rook",
    "Black Kingside Knight",
    "Black Kingside Bishop",
    "Black King",
    "Black A Pawn",
    "Black B Pawn",
    "Black C Pawn",
    "Black D Pawn",
    "Black E Pawn",
    "Black F Pawn",
    "Black G Pawn",
    "Black H Pawn",
    
    "ERROR"
  ])[pid.value];
}

function showPieceInfo(pid, captures)
{
  let name = getPieceName(pid);

  l("piece-img").src = createPieceImage(Module.colorOfPiece(pid), Module.typeOfPiece(pid)).src;
  resizeTextToLine(l("piece-name"), name, 0.9, 30);
  
  // get the piece to access it's capture list
  let piece = Game.game.board.getBoardPiece(pid.value)
  let numCaptures = piece.numCaptures;
  for(let i = 0; i < numCaptures; i++)
  {
    // find the id of the captured piece
    let cid = piece.getCapture(i);
    // create a box for the new lines
    let li = document.createElement("li");
    
    let img = createPieceImage(Module.colorOfPiece(cid), Module.typeOfPiece(cid));
    img.classList.add("info-piece-img");
    let nameBox = document.createElement("p");
    nameBox.classList.add("info-piece-name");
    
    li.appendChild(img);
    li.appendChild(nameBox);
    l("capture-list").appendChild(li);
    
    let name = getPieceName(cid);
    resizeTextToLine(nameBox, name, 0.8, 30);
    
  }
}

function clearPieceInfo()
{
  l("piece-img").src = "";
  l("piece-name").innerText = "";
  l("capture-list").innerText = "";
}

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
    // if we're on the sixth element, and this isn't the last one
    // checking i = 5 because zero-index
    if(i == 5 && numCaptures != 6)
    {
      // just display a note saying there's more
      let li = document.createElement("li");
      let nameBox = document.createElement("p");
      nameBox.classList.add("info-piece-name");
      li.appendChild(nameBox);
      l("capture-list").appendChild(li);
      
      let text = `+${numCaptures - i} more`;
      
      resizeTextToLine(nameBox, text, 0.8, 30);
      
      break;
    }
  
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
    
    // resize the li element to fit snuggly with it's contents.
    // not 100% sure why this is needed, my guess is that the li tries
    // making room for its bullets
    li.style.height = img.getBoundingClientRect().height + 5 + "px";
    
  }
}

function clearPieceInfo()
{
  l("piece-img").src = "";
  l("piece-name").innerText = "";
  l("capture-list").innerText = "";
}

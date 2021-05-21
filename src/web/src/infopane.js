function showPieceInfo(pid, captures)
{
  let name = ([
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

  resizeTextToLine(l("piece-name"), name, 0.9, 30);
}

function clearPieceInfo()
{
  l("piece-name").innerText = "";
}

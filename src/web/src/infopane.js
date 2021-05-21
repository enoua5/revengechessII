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

function resizeTextToLine(text, value, factor, max_iter)
{
  // reset the font size so we don't get fucky nonsense frame-to-frame
  let default_fontsize = getDefaultProperty(text.tagName, "fontSize");
  let fontsize = parseFloat(default_fontsize);
  text.style.fontSize = default_fontsize;
  // some fiddling to get the text the right size
  // for loop instead of while, to avoid hanging
  for(let i = 0; i < max_iter; i++)
  {
    text.innerText = "*";
    let natural_height = text.getBoundingClientRect().height;
    
    text.innerText = value;
    let current_height = text.getBoundingClientRect().height;
    
    // text is is one line, we break here
    if(current_height <= natural_height)
      break;
      
    // if the text is taking up two lines...
    fontsize *= factor;
    text.style.fontSize = fontsize+"px";
  }
}

function clearPieceInfo()
{
  l("piece-name").innerText = "";
}

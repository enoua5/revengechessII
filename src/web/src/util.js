function indexOfElement(el)
{
  let i = 0;
  while( (el = el.previousSibling) != null)
    i++;
  return i;
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

// https://stackoverflow.com/a/51597423/3154580
function getDefaultProperty(tag, prop)
{
  try
  {
    const elem = document.createElement(tag)
    document.body.appendChild(elem)
    const defaultStyles = Object.assign({}, window.getComputedStyle(elem))
    document.body.removeChild(elem)
    return defaultStyles[prop]
  }
  catch(err)
  {
    return err
  }
}

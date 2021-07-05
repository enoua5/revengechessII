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
    const defaultStyles = window.getComputedStyle(elem)
    let val = defaultStyles[prop];
    document.body.removeChild(elem)
    return val;
  }
  catch(err)
  {
    return err
  }
}

function selectRadio(className, selected, settingName)
{
  let els = document.getElementsByClassName(className);
  for(let e of els)
  {
    e.classList.remove("radio-selected");
  }
  selected.classList.add("radio-selected");
  Settings[settingName] = selected.dataset.value;
}

function shouldWhiteBeOnTop()
{
  if(Settings.orientation == "b")
    return true;
  if(Settings.orientation == "w")
    return false;
    
  if(Game.game.board.turn == Module.PlayerColor.WHITE)
    return false;
  return true;
}

function showWindow(id)
{
  l("pane").style.display = "block";
  l(id).style.display = "inline-block";
}
function hideWindows()
{
  let els = document.getElementsByClassName("window");
  for(let el of els)
  {
    el.style.display = ""; // style.css -> hidden
  }
  l("pane").style.display = ""; // style.css -> hidden
}

function setableStyles()
{
  return l("setable_styles").sheet.cssRules[0].style;
}

function resetColor(inputEl)
{
  let rule = inputEl.dataset.appliesto;
  setableStyles().setProperty("--"+rule, inputEl.dataset.resetcolor);
  setDefaultColor(inputEl);
}
function setDefaultColor(inputEl)
{
  let rule = inputEl.dataset.appliesto;
  let color = setableStyles().getPropertyValue("--"+rule).replace(/\s/, '');
  inputEl.value = color;
  inputEl.dataset.resetcolor = color;
}
function setThemeColor(inputEl)
{
  let rule = inputEl.dataset.appliesto;
  setableStyles().setProperty("--"+rule, inputEl.value);
}

function versionToString(v)
{
  return v.name + ", version " + String.fromCharCode(v.stage) + v.major + '.' + v.minor + '.' + v.patch;
}

function fillVersionNumbers()
{
  l("core-version").innerText = versionToString(Module.core_v());
  l("engine-version").innerText = versionToString(Module.engine_v());
}

function randomIntInclusive(min, max)
{
  min = Math.ceil(min);
  max = Math.floor(max) + 1;
  return Math.floor(Math.random() * (max - min) + min);
}

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
  else if(Settings.orientation == "w")
    return false;
  else if(Settings.orientation == "t")
  {
    let board = Game.game.board;
    let turn = board.turn;
    board.delete();
    if(turn == Module.PlayerColor.WHITE)
      return false;
    return true;
  }
  else
  {
    if(Server.in_online_game)
    {
      return (Server.play_as == "b");
    }
    else
    {
      if(Settings.ai.white.usingAI == Settings.ai.black.usingAI)
      {
        // if both are players or both are AI
        let board = Game.game.board;
        let turn = board.turn;
        board.delete();
        if(turn == Module.PlayerColor.WHITE)
          return false;
        return true;
      }
      else
      {
        // exactly one player is an AI
        return Settings.ai.white.usingAI;
      }
    }
  }
}

function resetAftergameOptions()
{
  l("tutorial-aftergame").style.display = "none";
  l("offline-aftergame").style.display = "none";
  l("online-aftergame").style.display = "none";
}

function confirmCloseOfOnlineGameIfNeeded()
{
  if(Server.in_online_game)
  {
    if(!confirm("This will close the online game you're currently in. Continue anyway?"))
      return false;
    Server.server.send(JSON.stringify({
      req: "close_game"
    }));
    Server.requested_close = true;
  }
  else
  {
    l("resign_button").style.display = "none";
    resetAftergameOptions();
    l("offline-aftergame").style.display = "";
  }

  return true;
}

function disposeOfCurrentGame()
{
  alreadyShowedResultScreen = false;
  l("show_results_button").style.display = "none";

  if(Game.game)
    Game.game.delete();
}

var selectedTutorial = {};
function selectTutorial(name)
{
  selectedTutorial = tutorials[name] || {};

  if(Object.keys(selectedTutorial).length == 0)
    return;

  l("tutorial-title").innerText = selectedTutorial["title"];
  l("tutorial-desc").innerText = selectedTutorial["description"];

  switchWindow("tutorial-expl-screen");
}
function startSelectedTutorial()
{
  if(Object.keys(selectedTutorial).length == 0)
    return;

  if(!confirmCloseOfOnlineGameIfNeeded())
    return;

  resetAftergameOptions();
  l("tutorial-aftergame").style.display = "";

  disposeOfCurrentGame();
  Game.game = new Module.Game();
  Game.game.board.delete();
  Game.game.board = new Module.Board(JSON.stringify(selectedTutorial["board_state"]));
  Game.game.clock.delete();
  Game.game.clock = new Module.Clock(1000, 1000, Module.IncrementMethod.NO_CLOCK,
                                     1000, 1000, Module.IncrementMethod.NO_CLOCK);
  copyAISettings(selectedTutorial["ai_settings"], Settings.ai.black);
  Settings.ai.white.usingAI = false;

  hideWindows();
  dispboard(Game.game.board);
  clearPrevMove();
}

function switchWindow(id)
{
  hideWindows();
  showWindow(id);
}

function showWindow(id)
{
  l("pane").style.display = "block";
  l(id).style.display = "inline-block";
  if(id=="game-explorer")
    updateGameList();
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
  delete changedColors[rule];
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
  setThemeColorByValue(rule, inputEl.value);
}
function setThemeColorByValue(rule, val)
{
  setableStyles().setProperty("--"+rule, val);
  changedColors[rule] = val;
}

function versionToString(v)
{
  return v.name + ", version " + (v.stage?String.fromCharCode(v.stage):'') + v.major + '.' + v.minor + '.' + v.patch;
}

function fillVersionNumbers()
{
  l("client-version").innerText = versionToString(client_version);
  l("core-version").innerText = versionToString(Module.core_v());
  l("engine-version").innerText = versionToString(Module.engine_v());
}

function randomIntInclusive(min, max)
{
  min = Math.ceil(min);
  max = Math.floor(max) + 1;
  return Math.floor(Math.random() * (max - min) + min);
}

function gameIsOver()
{
  let board = Game.game.board;
  let gr = board.getGameResult(true);
  board.delete();
  if(gr != Module.GameResult.ONGOING)
    return true;
  let clock = Game.game.clock;
  gr = clock.getResultFromFlag();
  clock.delete();
  if(gr != Module.GameResult.ONGOING)
    return true;
  return false;
}

function updatePrelimSets(element)
{
  if(element.dataset.enablesection)
  {
    if(element.checked)
      l(element.dataset.enablesection).classList.remove("setting-disable");
    else
      l(element.dataset.enablesection).classList.add("setting-disable");
  }
  if(element.dataset.disablesection)
  {
    if(!element.checked)
      l(element.dataset.disablesection).classList.remove("setting-disable");
    else
      l(element.dataset.disablesection).classList.add("setting-disable");
  }
  
  if(!prelimSettings[element.dataset.color])
    prelimSettings[element.dataset.color]={};
  
  let sets = prelimSettings[element.dataset.color];
  
  if(element.type != "checkbox")
    sets[element.dataset.appliesto] = element.value;
  else
    sets[element.dataset.appliesto] = element.checked;
    
  
}

function setupEnterToSubmit()
{
  let els = document.getElementsByClassName("enter-to-submit");
  for(let el of els)
  {
    el.addEventListener("keyup", function(e){
      e.preventDefault();
      if(e.keyCode == 13)
      {
        document.getElementById(this.dataset.submitid).click();
      }
    });
  }
}

function showSaveCode()
{
  l("save-board-box").innerText = Game.game.board.serialize();
}
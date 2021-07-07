var AI_status = {
  running: false,
  thinking_time: undefined,
  time_spent_before_thining: undefined
};

function getCurrentPlayerSets()
{
  return Settings.ai[ (Game.game.board.turn == Module.PlayerColor.WHITE) ? "white" : "black" ];
}

function getPieceTypeFromValue(value)
{
  let t = Module.PieceType;
  switch(value)
  {
    case t.PAWN.value:
      return t.PAWN;
    case t.ROOK.value:
      return t.ROOK;
    case t.KNIGHT.value:
      return t.KNIGHT;
    case t.BISHOP.value:
      return t.BISHOP;
    case t.QUEEN.value:
      return t.QUEEN;
    case t.KING.value:
      return t.KING;
    default:
      return t.NO;
  }
}

function acceptAIMove(e)
{
  let sets = getCurrentPlayerSets();
  
  if(!e.data || e.data.ranks == undefined || e.data.ranks.length == 0)
    return engineError({message: "Moves list empty"});
  if(e.data.turn != (Game.game.board.turn == Module.PlayerColor.WHITE))
    return engineError({message: "Move was somehow made before AI finished thinking."});
  if(!sets.usingAI)
    return engineError({message: "AI tried playing a move when it's not its turn."});
  
  let moveList = e.data.ranks;
  console.log(moveList)
  let scoreDelta = randomIntInclusive(sets.minDelta, sets.maxDelta);
  let topScore = moveList[0].score.score;
  let targetScore = topScore - scoreDelta;
  
  let closestMove = moveList[0];
  let nth = 0;
  for(let i of moveList)
  {
    if(i.score.score > targetScore)
    {
      if(i.score.score == closestMove.score.score)
        if(Math.random() < 0.5)
          continue;
      nth++;
      closestMove = i;
    }
    else
      break;
  }
  console.log("Best move:", moveList[0], "\nPlayed move:", closestMove, "\nTarget delta", scoreDelta, "\nActual delta", moveList[0].score.score-closestMove.score.score, "\nThis is the #"+nth+" top scoring move");
  
  let fromSquare = new Module.Square(closestMove.move.from);
  let toSquare =  new Module.Square(closestMove.move.to);
  let moveToMake = new Module.Move(
    fromSquare,
    toSquare,
    getPieceTypeFromValue(closestMove.move.promotion)
  );
  
  try
  {
    Game.game.commitMove(moveToMake);
  }
  catch(e)
  {
    return engineError(e);
  }
  
  AI_status.running = false;
  
  selectedSquares.prev_move = {
    from: fromSquare,
    to: toSquare
  };
  
  dispboard(Game.game.board);
}
function engineError(e)
{
  AI_status.running = false;
  l("ai-error").style.display = "";
  l("ai-error-msg").innerText = e.message;
  
  console.error(e.message);
}


function timeUntilFlag(color)
{
  let timeOnClock = 0;
  if(color == Module.PlayerColor.WHITE)
    timeOnClock = Game.game.clock.getWhiteTime();
  else
    timeOnClock = Game.game.clock.getBlackTime();
  
  return timeOnClock + Game.game.clock.getDelayLeft();
}

function getPlayerTimeLeft(player)
{
  if(player == Module.PlayerColor.BLACK)
    return Game.game.clock.getBlackTime();
  return Game.game.clock.getWhiteTime();
}
function getPlayerIncType(player)
{
  if(player == Module.PlayerColor.BLACK)
    return Game.game.clock.getBlackIncType();
  return Game.game.clock.getWhiteIncType();
}
function getPlayerIncAmount(player)
{
  if(player == Module.PlayerColor.BLACK)
    return Game.game.clock.getBlackIncrementAmount();
  return Game.game.clock.getWhiteIncrementAmount();
}


function getTimeForMove(player)
{
  // not using the getCurrentPlayerSets() function so we stick with the argument
  let sets = Settings.ai[ (player == Module.PlayerColor.WHITE) ? "white" : "black" ];;
  
  let time = 0;
  if(sets.limitMode == SearchLimits.CONSTANT_TIME)
  {
    time = sets.limitSeconds;
  }
  else if(sets.limitMode == SearchLimits.AUTOMATIC)
  {
    let halfMovesPlayed = Game.game.prevBoards.size() - 1; // -1 because it counts the current board as well
    let estimatedMovesLeft = Math.ceil(Math.max(80 - halfMovesPlayed, 10) / 2);
    
    let timeLeft = getPlayerTimeLeft(player);
    
    time = Math.round(timeLeft / estimatedMovesLeft);
    
    let incrementMethod = getPlayerIncType(player);
    if(incrementMethod == Module.IncrementMethod.DELAY || timeLeft - (time + 1000) > getPlayerIncAmount(player))
      time += getPlayerIncAmount(player);
  }
  else if(sets.limitMode == SearchLimits.CONSTANT_DEPTH)
    return sets.maxTime;
  
  
  if(sets.maxTime < time)
    time = sets.maxTime;
    
  let timeUntilBuffer = timeUntilFlag() - sets.flagBuffer;
  if(timeUntilBuffer < time)
    time = timeUntilBuffer;
    
  //console.log(`Spending ${time/1000} seconds thinking...`);
  return Math.max(time, 0);
    
}

function makeAIMove()
{
  if(gameIsOver())
    return;
  
  let sets = getCurrentPlayerSets();
  if(!sets.usingAI)
    return engineError({message: "AI tried playing a move when it's not its turn."});
    
  let time = getTimeForMove(Game.game.board.turn);
  
  let depth = 100;
  if(sets.limitMode == SearchLimits.CONSTANT_DEPTH)
    depth = sets.limitPlys;
    
  AI_status.running = true;
  AI_status.thinking_time = time;
  AI_status.time_spent_before_thining = Game.game.clock.getTimeSpentThisTurn();
  engine.postMessage({board: Game.game.board.serialize(), time: time, depth: depth});
  
}

var SearchLimits = {
  AUTOMATIC: 0,
  CONSTANT_TIME: 1,
  CONSTANT_DEPTH: 2,
};

function defaultEngineSettings()
{
  return {
    // AI is disabled by default
    usingAI: false, 
    
    // limiting settings, so they AI has to stop thinking eventually
    limitMode: SearchLimits.AUTOMATIC,
    maxTime: 60000, // 1 minute, even if desired time/depth is higher
    limitSeconds: 15000, // 15 seconds
    limitPlys: 6,
    flagBuffer: 500, // abort search if there's less than half a second left on the clock
    
    // AI tries to play a move that is between minDelta and maxDelta centipawns
    // worse than the best scoring move.
    minDelta: 0,
    maxDelta: 10,
    
    
  };
}

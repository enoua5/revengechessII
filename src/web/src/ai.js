function acceptAIMove(e)
{
  if(!e.data || e.data.ranks == undefined || e.data.ranks.length == 0)
    throw "Moves list empty";
  if(e.data.turn != (Game.game.board.turn == Module.PlayerColor.WHITE))
    throw "Move was somehow made before AI finished thinking."
  
  let moveList = e.data.ranks;
  let sets = Settings.ai[ (Game.game.board.turn == Module.PlayerColor.WHITE) ? "white" : "black" ];
  let scoreDelta = randomIntInclusive(sets.minDelta, sets.maxDelta);
  let topScore = moveList[0].score.score;
  let targetScore = topScore - scoreDelta;
  
  let closestMove = moveList[0];
  let nth = 1;
  for(let i of moveList)
  {
    if(i.score.score > targetScore)
    {
      nth++;
      closestMove = i;
    }
    else
      break;
  }
  console.log(nth);
  
  // TODO actually play the move
  console.log(closestMove);
}
function engineError(e)
{
  // TODO show error on info pane
  console.error(e);
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

function getTimeForMove(player)
{
  let sets = Settings.ai[ (player == Module.PlayerColor.WHITE) ? "white" : "black" ];
  
  let time = 0;
  if(sets.limitMode == SearchLimits.CONSTANT_TIME)
  {
    time = sets.limitSeconds;
  }
  else if(sets.limitMode == SearchLimits.AUTOMATIC)
  {
    time = 5000; // TODO write actual time management algorithm
  }
  else if(sets.limitMode == SearchLimits.CONSTANT_DEPTH)
    return sets.maxTime;
  
  
  if(sets.maxTime < time)
    time = sets.maxTime;
    
  let timeUntilBuffer = timeUntilFlag() - sets.flagBuffer;
  if(timeUntilBuffer < time)
    time = timeUntilBuffer;
    
  return Math.max(time, 0);
    
}

function makeAIMove()
{
  let time = getTimeForMove(Game.game.board.turn);
  
  let sets = Settings.ai[ (Game.game.board.turn == Module.PlayerColor.WHITE) ? "white" : "black" ];
  
  let depth = 100;
  if(sets.limitMode == SearchLimits.CONSTANT_DEPTH)
    depth = sets.limitPlys;
    
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

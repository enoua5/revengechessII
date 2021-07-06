var engine;

var Module = {
  onRuntimeInitialized: function() {
    engine = new Module.Engine();
  }
};

function objectifySearchResult(sr)
{
  return {
    score: sr.score,
    depth: sr.depth,
    result: sr.result.value
    // bm
    // pv
  };
}

onmessage = function(e) {
  if(!engine)
    throw "Engine not yet initialized";
    
  let board = new Module.Board(e.data.board)
  let ranks = engine.rankMoves(board, e.data.time/1000, e.data.depth);
  
  let ranksArr = [];
  for(let i = 0; i < ranks.size(); i++)
  {
    let move = ranks.get(i).move;
    ranksArr.push({
      score: objectifySearchResult(ranks.get(i).score),
      move: {
        from: move.from.toIndex(),
        to: move.to.toIndex(),
        promotion: move.promotion.value
      }
    });
  }
  
  ranksArr.sort((a,b) => (a.score.score < b.score.score ? 1 : -1));
  
  
  postMessage({ranks: ranksArr, turn: board.turn == Module.PlayerColor.WHITE });
}

importScripts("revengechess.js")

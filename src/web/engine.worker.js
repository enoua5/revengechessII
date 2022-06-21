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

function stringToSquare(s)
{
  s = s.toLowerCase();
  let f = s.charCodeAt(0)-97;
  let r = s.charCodeAt(1)-49;
  return new Module.Square(f,r);
}

function stringToMove(m)
{
  let from = m.slice(0,2);
  let to = m.slice(2,4);
  return new Module.Move(stringToSquare(from), stringToSquare(to));
}

onmessage = function(e) {
  if(!engine)
    throw "Engine not yet initialized";

  let board = new Module.Board(e.data.board)
  if(e.data.book_move == undefined)
  { 
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
    
    ranksArr.sort((a,b) => {
      if(a.score.score >= 500000 && b.score.score >= 500000)
        return a.score.depth > b.score.depth ? 1 : -1;
      if(a.score.score <= -500000 && b.score.score <= -500000)
        return a.score.depth < b.score.depth ? 1 : -1;
      
      return a.score.score < b.score.score ? 1 : -1
    });
    
    
    postMessage({ranks: ranksArr, turn: board.turn == Module.PlayerColor.WHITE });
  }
  else
  {
    let move = stringToMove(e.data.book_move);

    this.postMessage({ranks: [{
      score: {score: "book", depth: 0, result: 3},
      move: {
        from: move.from.toIndex(),
        to: move.to.toIndex(),
        promotion: move.promotion.value
      }
    }], turn: board.turn == Module.PlayerColor.WHITE});
  }

  board.delete();
}

importScripts("revengechess.js")

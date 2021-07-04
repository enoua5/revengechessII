var engine;

var Module = {
  onRuntimeInitialized: function() {
    engine = new Module.Engine();
  }
};

onmessage = function(e) {
  if(!engine)
    // TODO replace errorcodes with an enum or something
    postMessage({success: false, message: "Engine not yet initialized", errorCode: 1});
  
  postMessage(engine.solve(new Module.Board(e.data.board), e.data.time).bm.toString());
}

importScripts("revengechess.js")

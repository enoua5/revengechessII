#pragma once

#include "version.h"
#include "enum.h"
#include "game/board.h"
#include "game/square.h"
#include "game/clock.h"

#ifndef __EMSCRIPTEN__
  #include <stack>
#else
  #include <vector>
#endif

class Game
{
  public:
    static Version version;
  
    Game();
    
    Board board;
    Clock clock;
    
    #ifndef __EMSCRIPTEN__
      std::stack<Board> prevBoards;
    #else
      std::vector<Board> prevBoards;
    #endif
    
    void takeBack();
    void commitMove(Move);
    void startClock();
};

#pragma once

#include <vector>
#include "game/square.h"
#include "game/board.h"

class MoveOrdering
{
  public:
    MoveOrdering(Move best) : bestMove(best) {}
  
    Move bestMove;
    void order(Board, std::vector<Move>&);
};

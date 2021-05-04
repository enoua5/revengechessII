#include <iostream>


#include <vector>
#include <algorithm>
#include "game/square.h"
#include "game/board.h"
#include "engine/moveordering.h"

void MoveOrdering::order(Board board, std::vector<Move>& moves)
{
  std::vector<Move>::iterator it = std::find(moves.begin(), moves.end(), bestMove);
  if(it != moves.end())
    std::iter_swap(moves.begin(), it);
}

#include "game/game.h"
#include "game/board.h"
#include "game/moveinfo.h"

#include <iostream>

Version Game::version = Version("Revenge Chess Core", 'b', 2, 2, 0);

Game::Game()
{
  startClock();
// clock.toggle(); // start the clock for white
#ifndef __EMSCRIPTEN__
  prevBoards.push(board);
#else
  prevBoards.push_back(board);
#endif
}

void Game::startClock(PlayerColor pc)
{
  if (clock.isWhiteRunning() || clock.isBlackRunning())
    return;
  clock.toggle();
  if (pc == BLACK)
    clock.toggle();
}

void Game::takeBack()
{
  clock.toggle(); // I guess??? Since it's the other person's turn now
  if (prevBoards.size() > 1)
  {
#ifndef __EMSCRIPTEN__
    prevBoards.pop();
    board = prevBoards.top();
#else
    prevBoards.pop_back();
    board = prevBoards.back();
#endif
  }
}

void Game::commitMove(Move m)
{
  if (clock.getResultFromFlag() == ONGOING)
  {
    board = board.makeMove(m);
#ifndef __EMSCRIPTEN__
    prevBoards.push(board);
#else
    prevBoards.push_back(board);
#endif

    if (board.getGameResult() != ONGOING)
      clock.stop();
    else
      clock.toggle();
  }
  else
  {
    clock.stop();
  }
}

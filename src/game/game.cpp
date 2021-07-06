#include "game/game.h"
#include "game/board.h"

#include <iostream>

Version Game::version = Version("Revenge Chess Core", 'b', 2, 1, 0);

Game::Game()
{
  startClock();
  //clock.toggle(); // start the clock for white
  #ifndef __EMSCRIPTEN__
    prevBoards.push(board);
  #else
    prevBoards.push_back(board);
  #endif
}

void Game::startClock()
{
  if(clock.isWhiteRunning() || clock.isBlackRunning())
    return;
  clock.toggle();
}

void Game::takeBack()
{
  clock.toggle(); // I guess??? Since it's the other person's turn now
  if(prevBoards.size() > 1)
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
  if(clock.getResultFromFlag() == ONGOING)
    board = board.makeMove(m);
  else
  {
    clock.stop();
    return;
  }
  #ifndef __EMSCRIPTEN__
    prevBoards.push(board);
  #else
    prevBoards.push_back(board);
  #endif
  
  if(board.getGameResult() != ONGOING)
    clock.stop();
  else
    clock.toggle();
}

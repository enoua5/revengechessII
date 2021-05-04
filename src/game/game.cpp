#include "game/game.h"
#include "game/board.h"

#include <iostream>

Version Game::version = Version("Revenge Chess Core", 'b', 2, 1, 0);

Game::Game()
{
  #ifndef __EMSCRIPTEN__
    prevBoards.push(board);
  #else
    prevBoards.push_back(board);
  #endif
}

void Game::takeBack()
{
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
  board = board.makeMove(m);
  #ifndef __EMSCRIPTEN__
    prevBoards.push(board);
  #else
    prevBoards.push_back(board);
  #endif
}

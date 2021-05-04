#pragma once

#include "version.h"
#include "game/board.h"
#include "game/game.h"
#include "game/square.h"
#include "pieceID.h"
#include <iostream>
#include <string>

class UI
{
  public:
    static void printBoard(const Board& b);
    static void startUI(Game& game);
    static Move parseMove(std::string);
    static Version version;
};


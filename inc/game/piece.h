#pragma once

#include "enum.h"
#include "game/square.h"

class Piece
{
  public:
    Piece();
    Piece(PlayerColor color, PieceType type, char rank, char file);
    Piece(PieceIdentifier id);
  
    bool isOnBoard
    #ifndef __EMSCRIPTEN__
      : 1
    #endif
    ;
    bool hasMoved
    #ifndef __EMSCRIPTEN__
      : 1
    #endif
    ;
    PlayerColor color
    #ifndef __EMSCRIPTEN__
      : 1
    #endif
    ;
    PieceType type
    #ifndef __EMSCRIPTEN__
      : 4
    #endif
    ;
    unsigned char numCaptures
    #ifndef __EMSCRIPTEN__
      : 5
    #endif
    ;
    PieceIdentifier captures[17]; // all enemy pieces + 1 own piece
    
    Square home;
    Square current;
};


#pragma once
#include "debug.h"

#include "enum.h"
#include "game/square.h"
#include <string>
#include <exception>

class Square
{
  public:
    Square(); // only here to make other constructors shut up
    Square(unsigned char f, unsigned char r);
    Square(unsigned char i);

    unsigned char file
    #ifndef __EMSCRIPTEN__
      : 3
    #endif
    ;
    unsigned char rank
    #ifndef __EMSCRIPTEN__
      : 3
    #endif
    ;
    
    bool eq(const Square& other) const;
    
    unsigned char toIndex() const;
    Square translate(char df, char dr) const;
    bool isValid() const;
    
    std::string toString() const;
    
    unsigned char dist(const Square other) const;
    
  private:
    bool valid;
};

class Move
{
  public:
    Move(); // only here to make other constructors shut up
    Move(Square t, Square f,PieceType p = NO);
    
    Square from;
    Square to;
    PieceType promotion; // needed on very few moves, but hey
    
    #ifdef DEBUG
      std::string info;
      static std::string info_template;
    #endif
    
    bool eq(const Move& other) const;
    
    std::string toString() const;
};

bool operator==(const Square&, const Square&);
bool operator==(const Move&, const Move&);

class invalid_move : public std::exception {};



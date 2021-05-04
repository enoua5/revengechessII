#include <algorithm>
#include "game/square.h"

Square::Square() : file(0), rank(0) {}

Square::Square(unsigned char f, unsigned char r)
{
  rank = r;
  file = f;
  valid = (f < 8) && (r < 8);
}

Square::Square(unsigned char i)
{
  rank = i/8;
  file = i%8;
  valid = (i < 64);
}

bool Square::isValid() const
{
  return valid;
}

Move::Move(Square f, Square t, PieceType p) :  from(f.file, f.rank), to(t.file, t.rank)
{
  promotion = p;
}

Square Square::translate(char df, char dr) const
{
  return Square(((unsigned char)this->file)+df, ((unsigned char)this->rank)+dr);
}

std::string Square::toString() const
{
  if(!valid)
    return "INVALID";
  std::string r;
  r+='A'+this->file;
  r+='1'+this->rank;
  return r;
}

std::string Move::toString() const
{
  return from.toString()+to.toString();
}


unsigned char Square::dist(const Square other) const
{
  return std::max(
    7-std::abs(this->file - other.file),
    7-std::abs(this->rank - other.rank)
  );
}

bool Square::eq(const Square& other) const
{
  return *this == other;
}
    
bool operator==(const Square& a, const Square& b)
{
  return a.file == b.file && a.rank == b.rank;
}

bool operator==(const Move& a, const Move& b)
{
  return a.from == b.from && a.to == b.to; // ignoring promotion becuase it isn't relavant to the usage
}

unsigned char Square::toIndex() const
{
  return rank * 8 + file;
}

#include "debug.h"
#include "game/board.h"
#include "game/square.h"
#include "pieceID.h"
#include <vector>

//#define QUEEN_IS_COUNCELOR

bool Board::validDestination(const Square to, const Piece p) const
{
  if(!to.isValid()) return false;
  PieceIdentifier t = this->playField[to.toIndex()];
  if(t == EMPTY) return true;
  if(p.color == colorOfPiece(t)) return false;
  return true;
}

void Board::addPawnMoves(std::vector<Move>& moves, const Piece p) const
{
  #ifdef DEBUG
    Move::info_template = "PAWN";
  #endif

  char dir = p.color == WHITE ? 1 : -1;
  
  Square from = p.current;
  Square to;
  
  //forward 1
  to = from.translate(0, dir);
  if(validDestination(to, p) && this->playField[to.toIndex()] == EMPTY)
  {
    if(to.rank == 0 || to.rank == 7)
    {
      //promotion
      for(char pro = PAWN; pro <= QUEEN; pro++)
        moves.push_back(Move(from, to, (PieceType)pro));
    }
    else
    {
      moves.push_back(Move(from, to));
    }
    
    // forward 2
    to = to.translate(0, dir);
    if(!p.hasMoved && validDestination(to, p) && this->playField[to.toIndex()] == EMPTY)
      moves.push_back(Move(from, to));
  }
  
  // attacks
  Square enPassantSquare = Square(this->pawnEnPassantFile, dir == 1 ? 5 : 2);
  
  to = from.translate(-1, dir);
  if(to.isValid())
  {
    PieceIdentifier id = this->playField[to.toIndex()];
    if((this->pawnEnPassantFile != NONE && to == enPassantSquare) || (id != EMPTY && colorOfPiece(id) != p.color))
      moves.push_back(Move(from, to));
  }
  
  to = from.translate(1, dir);
  if(to.isValid())
  {
    PieceIdentifier id = this->playField[to.toIndex()];
    if((this->pawnEnPassantFile != NONE && to == enPassantSquare) || (id != EMPTY && colorOfPiece(id) != p.color))
      moves.push_back(Move(from, to));
  }
}
void Board::addRookMoves(std::vector<Move>& moves, const Piece p) const
{
  #ifdef DEBUG
    Move::info_template = "ROOK";
  #endif

  Square from = p.current;
  

  Square to;
  // up
  for(char o = 1; /*handled by a break*/; o++)
  {
    to = from.translate(0, o);
    
    if(!validDestination(to, p))
      break;
      
    moves.push_back(Move(from, to));
    
    if(this->playField[to.toIndex()] != EMPTY)
      break;
  }
  // down
  for(char o = -1; /*handled by a break*/; o--)
  {
    to = from.translate(0, o);
    
    if(!validDestination(to, p))
      break;
      
    moves.push_back(Move(from, to));
    
    if(this->playField[to.toIndex()] != EMPTY)
      break;
  }
  // right
  for(char o = 1; /*handled by a break*/; o++)
  {
    to = from.translate(o, 0);
    
    if(!validDestination(to, p))
      break;
      
    moves.push_back(Move(from, to));
    
    if(this->playField[to.toIndex()] != EMPTY)
      break;
  }
  // left
  for(char o = -1; /*handled by a break*/; o--)
  {
    to = from.translate(o, 0);
    
    if(!validDestination(to, p))
      break;
      
    moves.push_back(Move(from, to));
    
    if(this->playField[to.toIndex()] != EMPTY)
      break;
  }
}
void Board::addKnightMoves(std::vector<Move>& moves, const Piece p) const
{
  #ifdef DEBUG
    Move::info_template = "KNIGHT";
  #endif

  Square from = p.current;
  Square to;
  
  to = from.translate(2,1);
  if(validDestination(to, p))
    moves.push_back(Move(from, to));
  
  to = from.translate(2,-1);
  if(validDestination(to, p))
    moves.push_back(Move(from, to));
  
  to = from.translate(-2,1);
  if(validDestination(to, p))
    moves.push_back(Move(from, to));
  
  to = from.translate(-2,-1);
  if(validDestination(to, p))
    moves.push_back(Move(from, to));
  
  to = from.translate(1,2);
  if(validDestination(to, p))
    moves.push_back(Move(from, to));
  
  to = from.translate(1,-2);
  if(validDestination(to, p))
    moves.push_back(Move(from, to));
  
  to = from.translate(-1,2);
  if(validDestination(to, p))
    moves.push_back(Move(from, to));
  
  to = from.translate(-1,-2);
  if(validDestination(to, p))
    moves.push_back(Move(from, to));
  
}
void Board::addBishopMoves(std::vector<Move>& moves, const Piece p) const
{
  #ifdef DEBUG
    Move::info_template = "BISHOP";
  #endif

  Square from = p.current;
  

  Square to;
  
  // up right
  for(char o = 1; /*handled by a break*/; o++)
  {
    to = from.translate(o, o);
    
    if(!validDestination(to, p))
      break;
      
    moves.push_back(Move(from, to));
    
    if(this->playField[to.toIndex()] != EMPTY)
      break;
  }
  // up left
  for(char o = 1; /*handled by a break*/; o++)
  {
    to = from.translate(o, -o);
    
    if(!validDestination(to, p))
      break;
      
    moves.push_back(Move(from, to));
    
    if(this->playField[to.toIndex()] != EMPTY)
      break;
  }
  
  // down right
  for(char o = 1; /*handled by a break*/; o++)
  {
    to = from.translate(-o, o);
    
    if(!validDestination(to, p))
      break;
      
    moves.push_back(Move(from, to));
    
    if(this->playField[to.toIndex()] != EMPTY)
      break;
  }
  // down left
  for(char o = 1; /*handled by a break*/; o++)
  {
    to = from.translate(-o, -o);
    
    if(!validDestination(to, p))
      break;
      
    moves.push_back(Move(from, to));
    
    if(this->playField[to.toIndex()] != EMPTY)
      break;
  }
}
void Board::addQueenMoves(std::vector<Move>& moves, const Piece p) const
{
  #ifdef DEBUG
    Move::info_template = "QUEEN";
  #endif

  Square from = p.current;
  Square to;
  
  #ifdef QUEEN_IS_COUNCELOR
    to = from.translate(1,1);
    if(validDestination(to, p))
      moves.push_back(Move(from, to));
    to = from.translate(1,-1);
    if(validDestination(to, p))
      moves.push_back(Move(from, to));
    to = from.translate(-1,1);
    if(validDestination(to, p))
      moves.push_back(Move(from, to));
    to = from.translate(-1,-1);
    if(validDestination(to, p))
      moves.push_back(Move(from, to));
  #else
    // up
    for(char o = 1; /*handled by a break*/; o++)
    {
      to = from.translate(0, o);
      
      if(!validDestination(to, p))
        break;
        
      moves.push_back(Move(from, to));
      
      if(this->playField[to.toIndex()] != EMPTY)
        break;
    }
    // down
    for(char o = -1; /*handled by a break*/; o--)
    {
      to = from.translate(0, o);
      
      if(!validDestination(to, p))
        break;
        
      moves.push_back(Move(from, to));
      
      if(this->playField[to.toIndex()] != EMPTY)
        break;
    }
    // right
    for(char o = 1; /*handled by a break*/; o++)
    {
      to = from.translate(o, 0);
      
      if(!validDestination(to, p))
        break;
        
      moves.push_back(Move(from, to));
      
      if(this->playField[to.toIndex()] != EMPTY)
        break;
    }
    // left
    for(char o = -1; /*handled by a break*/; o--)
    {
      to = from.translate(o, 0);
      
      if(!validDestination(to, p))
        break;
        
      moves.push_back(Move(from, to));
      
      if(this->playField[to.toIndex()] != EMPTY)
        break;
    }
    
    // up right
    for(char o = 1; /*handled by a break*/; o++)
    {
      to = from.translate(o, o);
      
      if(!validDestination(to, p))
        break;
        
      moves.push_back(Move(from, to));
      
      if(this->playField[to.toIndex()] != EMPTY)
        break;
    }
    // up left
    for(char o = 1; /*handled by a break*/; o++)
    {
      to = from.translate(o, -o);
      
      if(!validDestination(to, p))
        break;
        
      moves.push_back(Move(from, to));
      
      if(this->playField[to.toIndex()] != EMPTY)
        break;
    }
    
    // down right
    for(char o = 1; /*handled by a break*/; o++)
    {
      to = from.translate(-o, o);
      
      if(!validDestination(to, p))
        break;
        
      moves.push_back(Move(from, to));
      
      if(this->playField[to.toIndex()] != EMPTY)
        break;
    }
    // down left
    for(char o = 1; /*handled by a break*/; o++)
    {
      to = from.translate(-o, -o);
      
      if(!validDestination(to, p))
        break;
        
      moves.push_back(Move(from, to));
      
      if(this->playField[to.toIndex()] != EMPTY)
        break;
    }
  #endif
}
void Board::addKingMoves(std::vector<Move>& moves, const Piece p) const
{
  #ifdef DEBUG
    Move::info_template = "KING:NORMAL";
  #endif

  Square from = p.current;
  Square to;
  
  to = from.translate(0,1);
  if(validDestination(to, p))
    moves.push_back(Move(from, to));
  to = from.translate(0,-1);
  if(validDestination(to, p))
    moves.push_back(Move(from, to));
  to = from.translate(1,0);
  if(validDestination(to, p))
    moves.push_back(Move(from, to));
  to = from.translate(-1,0);
  if(validDestination(to, p))
    moves.push_back(Move(from, to));
  to = from.translate(1,1);
  if(validDestination(to, p))
    moves.push_back(Move(from, to));
  to = from.translate(1,-1);
  if(validDestination(to, p))
    moves.push_back(Move(from, to));
  to = from.translate(-1,1);
  if(validDestination(to, p))
    moves.push_back(Move(from, to));
  to = from.translate(-1,-1);
  if(validDestination(to, p))
    moves.push_back(Move(from, to));
  
  // castle
  if(!p.hasMoved)
  {
    if(!this->pieces[idFromStats(p.color, ROOK, A)].hasMoved)
    {
      #ifdef DEBUG
        Move::info_template = "KING:CASTLE:QUEENSIDE";
      #endif

      unsigned char ind = from.toIndex();
      bool all_empty = true;
      for(char i = -1; i >= -3; i--)
      {
        if(this->playField[ind+i] != EMPTY)
        {
          all_empty = false;
          break;
        }
      }
      if(all_empty)
        moves.push_back(Move(from, from.translate(-2, 0)));
    }
    if(!this->pieces[idFromStats(p.color, ROOK, H)].hasMoved)
    {
      #ifdef DEBUG
        Move::info_template = "KING:CASTLE:KINGSIDE";
      #endif
      
      unsigned char ind = from.toIndex();
      bool all_empty = true;
      for(char i = 1; i <= 2; i++)
      {
        if(this->playField[ind+i] != EMPTY)
        {
          all_empty = false;
          break;
        }
      }
      if(all_empty)
        moves.push_back(Move(from, from.translate(2, 0)));
    }
  }
}

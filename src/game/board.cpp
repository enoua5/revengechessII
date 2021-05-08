#include "game/board.h"
#include "game/square.h"
#include "enum.h"
#include "pieceID.h"
#include <vector>
#include <queue>
#include <iterator>
#include <exception>

Board::Board()
{
  turn = WHITE;
  pawnEnPassantFile = NONE;
  halfMoveClock = 0;
  
  PieceIdentifier startPos[64] = {
    //r1
    WHITE_QUEENSIDE_ROOK,
    WHITE_QUEENSIDE_KNIGHT,
    WHITE_QUEENSIDE_BISHOP,
    WHITE_QUEEN,
    WHITE_KING,
    WHITE_KINGSIDE_BISHOP,
    WHITE_KINGSIDE_KNIGHT,
    WHITE_KINGSIDE_ROOK,
    //r2
    WHITE_A_PAWN,
    WHITE_B_PAWN,
    WHITE_C_PAWN,
    WHITE_D_PAWN,
    WHITE_E_PAWN,
    WHITE_F_PAWN,
    WHITE_G_PAWN,
    WHITE_H_PAWN,
    //r3
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    //r4
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    //r5
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    //r6
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    //r7
    BLACK_A_PAWN,
    BLACK_B_PAWN,
    BLACK_C_PAWN,
    BLACK_D_PAWN,
    BLACK_E_PAWN,
    BLACK_F_PAWN,
    BLACK_G_PAWN,
    BLACK_H_PAWN,
    //r8
    BLACK_QUEENSIDE_ROOK,
    BLACK_QUEENSIDE_KNIGHT,
    BLACK_QUEENSIDE_BISHOP,
    BLACK_QUEEN,
    BLACK_KING,
    BLACK_KINGSIDE_BISHOP,
    BLACK_KINGSIDE_KNIGHT,
    BLACK_KINGSIDE_ROOK
  };
  
  for(int i = 0; i < 64; i ++)
    playField[i]=startPos[i];
    
  pieces[WHITE_QUEENSIDE_ROOK] = Piece(WHITE, ROOK, 0, 0);
  pieces[WHITE_QUEENSIDE_KNIGHT] = Piece(WHITE, KNIGHT, 1, 0);
  pieces[WHITE_QUEENSIDE_BISHOP] = Piece(WHITE, BISHOP, 2, 0);
  pieces[WHITE_QUEEN] = Piece(WHITE, QUEEN, 3, 0);
  pieces[WHITE_KING] = Piece(WHITE, KING, 4, 0);
  pieces[WHITE_KINGSIDE_BISHOP] = Piece(WHITE, BISHOP, 5, 0);
  pieces[WHITE_KINGSIDE_KNIGHT] = Piece(WHITE, KNIGHT, 6, 0);
  pieces[WHITE_KINGSIDE_ROOK] = Piece(WHITE, ROOK, 7, 0);
  
  pieces[WHITE_A_PAWN] = Piece(WHITE, PAWN, 0, 1);
  pieces[WHITE_B_PAWN] = Piece(WHITE, PAWN, 1, 1);
  pieces[WHITE_C_PAWN] = Piece(WHITE, PAWN, 2, 1);
  pieces[WHITE_D_PAWN] = Piece(WHITE, PAWN, 3, 1);
  pieces[WHITE_E_PAWN] = Piece(WHITE, PAWN, 4, 1);
  pieces[WHITE_F_PAWN] = Piece(WHITE, PAWN, 5, 1);
  pieces[WHITE_G_PAWN] = Piece(WHITE, PAWN, 6, 1);
  pieces[WHITE_H_PAWN] = Piece(WHITE, PAWN, 7, 1);
  
  pieces[BLACK_QUEENSIDE_ROOK] = Piece(BLACK, ROOK, 0, 7);
  pieces[BLACK_QUEENSIDE_KNIGHT] = Piece(BLACK, KNIGHT, 1, 7);
  pieces[BLACK_QUEENSIDE_BISHOP] = Piece(BLACK, BISHOP, 2, 7);
  pieces[BLACK_QUEEN] = Piece(BLACK, QUEEN, 3, 7);
  pieces[BLACK_KING] = Piece(BLACK, KING, 4, 7);
  pieces[BLACK_KINGSIDE_BISHOP] = Piece(BLACK, BISHOP, 5, 7);
  pieces[BLACK_KINGSIDE_KNIGHT] = Piece(BLACK, KNIGHT, 6, 7);
  pieces[BLACK_KINGSIDE_ROOK] = Piece(BLACK, ROOK, 7, 7);
  
  pieces[BLACK_A_PAWN] = Piece(BLACK, PAWN, 0, 6);
  pieces[BLACK_B_PAWN] = Piece(BLACK, PAWN, 1, 6);
  pieces[BLACK_C_PAWN] = Piece(BLACK, PAWN, 2, 6);
  pieces[BLACK_D_PAWN] = Piece(BLACK, PAWN, 3, 6);
  pieces[BLACK_E_PAWN] = Piece(BLACK, PAWN, 4, 6);
  pieces[BLACK_F_PAWN] = Piece(BLACK, PAWN, 5, 6);
  pieces[BLACK_G_PAWN] = Piece(BLACK, PAWN, 6, 6);
  pieces[BLACK_H_PAWN] = Piece(BLACK, PAWN, 7, 6);
}

Board::Board(const Board* other)
{
  turn = other->turn;
  pawnEnPassantFile = other->pawnEnPassantFile;
  halfMoveClock = other->halfMoveClock;
  for(int i = 0; i < 64; i++)
    playField[i] = other->playField[i];
  for(int i = 0; i < 32; i++)
    pieces[i] = other->pieces[i];
}

std::vector<Move> Board::getValidMoves(PieceIdentifier id) const
{
  std::vector<Move> moves;
  
  Piece p = this->pieces[id];
  if(!p.isOnBoard)
    return moves; // no moves for a captured piece
  
  switch(p.type)
  {
    case PAWN:
      addPawnMoves(moves, p);
    break;
    case ROOK:
      addRookMoves(moves, p);
    break;
    case KNIGHT:
      addKnightMoves(moves, p);
    break;
    case BISHOP:
      addBishopMoves(moves, p);
    break;
    case QUEEN:
      addQueenMoves(moves, p);
    break;
    case KING:
      addKingMoves(moves, p);
    break;
    
    default: break;
  }
  return moves;
}

std::vector<Move> Board::getValidMoves() const
{
  return getValidMoves(this->turn);
}

std::vector<Move> Board::getValidMoves(PlayerColor c) const
{
  std::vector<Move> moves;
  
  if(getGameResult(false) != ONGOING)
    return moves;
  
  int start = 0;
  int end = 16;
  
  if(c == BLACK)
  {
    start = 16;
    end = 32;
  }
    
  
  for(int i = start; i < end; i++) // for each piece
  {
    // add that piece's moves
    std::vector<Move> newMoves = getValidMoves((PieceIdentifier) i);
    moves.insert(moves.end(), newMoves.begin(), newMoves.end());
  }
  
  return moves;
}

Board Board::makeMove(const Move move, bool trusted) const
{
  Board next(this);
  next.gameMoves.push_back(move);
  
  // if we didn't already check the validity, do that now
  if(!trusted)
  {
    auto v = getValidMoves();
    bool valid = false;
    
    std::vector<Move> :: iterator i;
    for(i = v.begin(); i != v.end(); i++)
    {
      if(move == *i)
      {
        valid = true;
        break;
      }
    }
    
    if(!valid)
      throw invalid_move();
  }
  
  // increment the 75-move rule clock
  next.halfMoveClock++;
  
  // save these to reduce recalcs
  PieceIdentifier attacking = this->playField[move.from.toIndex()];
  PieceIdentifier defending = this->playField[move.to.toIndex()];
  
  // various features of this are useful
  Piece* mover = &next.pieces[attacking];
  
  // some more precalcs to figure out what's going on
  bool pawnMove = mover->type == PAWN; // ignore promoted pawns
  bool enPassant = pawnMove && (defending == EMPTY) && (move.from.file != move.to.file);
  
  if(pawnMove)
  {
    next.halfMoveClock = 0;
    // if they've made it to the end of the board
    if(move.to.rank == 0 || move.to.rank == 7) mover->type = move.promotion;
    // if they've used the double move
    else if((move.from.rank == 1 && move.to.rank == 3) 
          ||(move.from.rank == 6 && move.to.rank == 4))
            next.pawnEnPassantFile = (File)move.to.file;
    else next.pawnEnPassantFile = NONE;
  }
  else next.pawnEnPassantFile = NONE;
  
  // check if we're castling
  bool castled = false;
  if(typeOfPiece(attacking) == KING && !mover->hasMoved)
  {
    // this is the king's first move
    // ignores promoted pawns
    if(move.to.file == 2)
    {
      // queenside castle
      
      // move rook beside king
      next.playField[move.from.toIndex()-1] = next.playField[move.from.toIndex()-4];
      // move king
      next.playField[move.from.toIndex()] = attacking;
      
      // update the rook's position now, because it won't be later, unlike the king's
      next.pieces[next.playField[move.from.toIndex()-1]].current = Square(move.from.toIndex()-1);
      
      next.playField[move.from.toIndex()-4] = EMPTY;
      next.playField[move.from.toIndex()] = EMPTY;
      
    }
    else if(move.to.file == 6)
    {
      // king side castle
      
      // move rook beside king
      next.playField[move.from.toIndex()+1] = next.playField[move.from.toIndex()+3];
      // move king
      next.playField[move.from.toIndex()] = attacking;
      
      // update the rook's position now, because it won't be later, unlike the king's
      next.pieces[next.playField[move.from.toIndex()+1]].current = Square(move.from.toIndex()+1);
      
      next.playField[move.from.toIndex()+3] = EMPTY;
      next.playField[move.from.toIndex()] = EMPTY;
    }
  }
  if(!castled)
  {
    next.playField[move.to.toIndex()] = attacking;
    next.playField[move.from.toIndex()] = EMPTY;
  }
  
  
  // update the mover's stats
  mover->hasMoved = true;
  mover->current = move.to;
  
  if(defending != EMPTY
     || enPassant)
  {
    // a capture has happened
    next.halfMoveClock = 0;
    
    PieceIdentifier captured;
    if(!enPassant)
    {
      captured = defending;
    }
    else
    {
      char offset = this->turn == WHITE ? -8 : 8; // up or down the board one row from the mover is captured 
      captured = this->playField[move.to.toIndex()+offset];
      
      next.playField[move.to.toIndex()+offset] = EMPTY;
    }
    
    mover->captures[mover->numCaptures++] = captured;
    next.pieces[captured].isOnBoard = false;
    std::queue<PieceIdentifier> toRespawn;
    for(unsigned char i = 0; i < next.pieces[captured].numCaptures; i++)
      toRespawn.push(next.pieces[captured].captures[i]);
    
    while(!toRespawn.empty())
    {
      PieceIdentifier resp = toRespawn.front();
      //remake it
      next.pieces[resp] = Piece(resp);
      unsigned char respPoint = next.pieces[resp].home.toIndex();
      toRespawn.pop();
      
      PieceIdentifier spawnOver = next.playField[respPoint];
      if(spawnOver != EMPTY)
      {
        next.pieces[resp].captures[next.pieces[resp].numCaptures++] = spawnOver;
        next.pieces[spawnOver].isOnBoard = false;
        
        for(unsigned char i = 0; i < next.pieces[spawnOver].numCaptures; i++)
        {
          toRespawn.push(next.pieces[spawnOver].captures[i]);
        }
      }
      next.playField[respPoint] = resp;
    }
  }
    
  next.turn = (PlayerColor)!next.turn;
  return next;
  
}

bool Board::hasKing(PlayerColor c) const
{
  PieceIdentifier king = WHITE_KING;
  PieceIdentifier aPawn = WHITE_A_PAWN;
  if(c == BLACK)
  {
    king = BLACK_KING;
    aPawn = BLACK_A_PAWN;
  }
  
  if(this->pieces[king].isOnBoard)
    return true;
    
  // check if pawns are promoted to kings
  for(unsigned char i = 0; i < 8; i++)
  {
    Piece p = this->pieces[aPawn+i];
    if(p.isOnBoard && p.type == KING)
      return true;
  }
  
  return false;
}

GameResult Board::getGameResult(bool checkMoves) const
{
  bool whiteKing = hasKing(WHITE);
  bool blackKing = hasKing(BLACK);
  
  if(!whiteKing && !blackKing)
    return STALEMATE;
  else if(!whiteKing)
    return BLACK_VICTORY;
  else if (!blackKing)
    return WHITE_VICTORY;
  if(halfMoveClock >= 150)
    return STALEMATE;
  
  if(checkMoves)
    if(getValidMoves(turn).size() == 0)
      return STALEMATE;
    
  return ONGOING;
}

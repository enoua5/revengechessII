#include "pieceID.h"
#include "game/board.h"
#include "game/square.h"

PlayerColor colorOfPiece(PieceIdentifier id)
{
  return id < 16 ? WHITE : BLACK;
}

PieceType typeOfPiece(PieceIdentifier id)
{
  if((id % 16) >= 8) return PAWN;
  switch(id % 4)
  {
    case 0: return ROOK;
    case 1: return KNIGHT;
    case 2: return BISHOP;
  }
  // it's a royal, but which...
  if(id % 8 == 3) return QUEEN;
  return KING;
}

BoardSide homeBoardSideOfPiece(PieceIdentifier id)
{
  return (id % 8) < 4 ? QUEENSIDE : KINGSIDE;
}

File homeFileOfPiece(PieceIdentifier id)
{
  // pieces were sorted by home side rather than file, so this one is a bit more involved
  char s = id % 16;
  // pawns and queensides are fine
  if(s >= 8 || s < 4) return (File)(id % 8);
  // these ones need to be flipped
  // (3-(id % 4))+4 -> 7 - (id % 4)
  return (File)(7 - (id % 4));
}

PieceIdentifier idFromStats(PlayerColor c, PieceType t, File f){
  char id = (c == WHITE) ? 0 : 16;
  if(t == PAWN) return (PieceIdentifier) (id + 8 + f);
  else
  {
    if(f < 4) return (PieceIdentifier) (id + f);
    return (PieceIdentifier) (id + (7 - (f % 4)));
  }
  
}

char pieceToken(Board b, PieceIdentifier id)
{
  char t = '.';
  if(id == EMPTY) return t;
  
  switch(b.pieces[id].type)
  {
    case PAWN: t = 'P';
    break;
    case ROOK: t = 'R';
    break;
    case KNIGHT: t = 'N';
    break;
    case BISHOP: t = 'B';
    break;
    case KING: t = 'K';
    break;
    case QUEEN: t = 'Q';
    break;
    default:
    break;
  }
  
  if(colorOfPiece(id) == BLACK) t+=32; // lowercase
  
  return t;
}

Square homeOfPiece(PieceIdentifier id)
{
  unsigned char f = (unsigned char)homeFileOfPiece(id);
  unsigned char rt = id/8;
  switch(rt)
  {
    case 0:
      return Square(f, 0);
    case 1:
      return Square(f, 1);
    case 2:
      return Square(f, 7);
    case 3:
      return Square(f, 6);
  }
  return Square(-1);
}

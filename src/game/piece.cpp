#include "game/piece.h"
#include "pieceID.h"

Piece::Piece() : isOnBoard(false), home(0, 0), current(0, 0) {}

Piece::Piece(PlayerColor c, PieceType t, char file, char rank) : home(file, rank), current(file, rank)
{
  isOnBoard = true;
  color = c;
  type = t;
  hasMoved = false;
  numCaptures = 0;
}

Piece::Piece(PieceIdentifier id)
{
  isOnBoard = true;
  color = colorOfPiece(id);
  type = typeOfPiece(id);
  hasMoved = false;
  numCaptures = 0;
  home = homeOfPiece(id);
  current = home;
}

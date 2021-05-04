#pragma once

#include "enum.h"
#include "game/board.h"
#include "game/square.h"

PlayerColor colorOfPiece(PieceIdentifier);
PieceType typeOfPiece(PieceIdentifier);
BoardSide homeBoardSideOfPiece(PieceIdentifier);
File homeFileOfPiece(PieceIdentifier);
Square homeOfPiece(PieceIdentifier);
PieceIdentifier idFromStats(PlayerColor, PieceType, File);

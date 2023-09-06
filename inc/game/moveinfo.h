#ifndef MOVEINFO_H
#define MOVEINFO_H

#include <vector>

#include "square.h"
#include "enum.h"

typedef std::vector<PieceIdentifier> RespawnWave;

class Board;

struct MoveInfo
{
    Move move;
    PieceIdentifier movedPiece;
    bool wasCapture;
    bool valid;
    std::vector<RespawnWave> respawns;
};

#endif
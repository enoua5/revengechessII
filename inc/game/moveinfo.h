#ifndef MOVEINFO_H
#define MOVEINFO_H

#include <vector>

#include "square.h"
#include "enum.h"

typedef std::vector<PieceIdentifier> RespawnWave;

class Board;

struct MoveInfo
{
    ~MoveInfo();

    Move move;
    // this circular dependancy drove me crazy
    // is that important to comment on? probably not.
    Board *newBoard;
    PieceIdentifier movedPiece;
    bool wasCapture;
    bool valid;
    std::vector<RespawnWave> respawns;
};

#endif
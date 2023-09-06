#include "game/moveinfo.h"
#include "game/board.h"

MoveInfo::~MoveInfo()
{
    delete this->newBoard;
}
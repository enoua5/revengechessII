#pragma once

#include "enum.h"
#include "game/piece.h"
#include "game/square.h"
#include <vector>
#include <queue>
#include <iterator>
#include <exception>

class Board
{
  public:
    Board();
    Board(const Board*);
    
    //#ifdef __EMSCRIPTEN__
      // the board has to be serialized to be passed to the Web Worker
      std::string serialize();
      Board(std::string); // deserialize
    //#endif
  
    PlayerColor turn 
    #ifndef __EMSCRIPTEN__
      : 1
    #endif
    ;
    File pawnEnPassantFile // NONE if there was no double-move last turn. Otherwise, the pawn two spaces out from the color whose turn it is not (ie, just moved) is vulnerable
    #ifndef __EMSCRIPTEN__
      : 4
    #endif
    ;
    
    unsigned char halfMoveClock // 75-move rule.
    #ifndef __EMSCRIPTEN__
      : 7
    #endif
    ;
    
    
    PieceIdentifier playField[64];
    Piece pieces[32];
    
    PieceIdentifier getPlayField(unsigned char i);
    Piece getBoardPiece(unsigned char i);
    
    std::vector<Move> getValidMoves() const;
    std::vector<Move> getValidMoves(PlayerColor) const;
    std::vector<Move> getValidMoves(PieceIdentifier) const; // get all moves for a particular piece.
    Board makeMove(const Move, bool trusted = false) const;
    
    GameResult getGameResult(bool checkMoves = true) const;
    bool hasKing(PlayerColor) const;
    
    bool validDestination(const Square, const Piece) const;
    void addPawnMoves(std::vector<Move>&, const Piece) const;
    void addRookMoves(std::vector<Move>&, const Piece) const;
    void addKnightMoves(std::vector<Move>&, const Piece) const;
    void addBishopMoves(std::vector<Move>&, const Piece) const;
    void addQueenMoves(std::vector<Move>&, const Piece) const;
    void addKingMoves(std::vector<Move>&, const Piece) const;
};


char pieceToken(Board b, PieceIdentifier id);

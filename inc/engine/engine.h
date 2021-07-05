#pragma once

//#define DEBUG

#include <list>
#include <vector>
#include <chrono>
#include "game/square.h"
#include "game/board.h"
#include "enum.h"
#include "version.h"

typedef std::chrono::high_resolution_clock clock_res;
typedef std::chrono::time_point<clock_res> Time;

struct SearchResult
{
  SearchResult() : score(0), result(ABORT), depth(0) {}
  SearchResult(int s, GameResult r, int d) : score(s), result(r), depth(d) {}
  int score;
  GameResult result;
  int depth;
  Move bm;
  #ifndef __EMSCRIPTEN__
    std::list<Move> pv;
  #else
    std::vector<Move> pv;
  #endif
  
};

struct MoveScore
{
  MoveScore() {}
  MoveScore(Move m, SearchResult sr) : move(m), score(sr) {}
  Move move;
  SearchResult score;
};

class Engine
{
  public:
    SearchResult solve(const Board&, const Time, const int);
    SearchResult solve(const Board&, const float seconds = 15.0f);
    std::vector<MoveScore> rankMoves(const Board&, const float maxSeconds, const int maxDepth);
    
    static Version version;
    
    int static_eval(const Board&) const;
    
    
    static const int pos_inf = 1000000;
    static const int neg_inf = -Engine::pos_inf;
    
    volatile bool abort;
  private:
    SearchResult alpha_beta(const Board& board, int depth, int alpha, int beta);
    int eval_piece(const Board&, const Piece) const;
    
    Time endtime;
    #ifndef __EMSCRIPTEN__
      std::list<Move> pv;
    #else
      std::vector<Move> pv;
    #endif
    
    static const int PAWN_VAL = 100;
    static const int BISHOP_VAL = 340;
    static const int ROOK_VAL = 500;
    static const int KNIGHT_VAL = 325;
    #ifdef QUEEN_IS_COUNCELOR
    static const int QUEEN_VAL = 900;
    #else
    static const int QUEEN_VAL = 200;
    #endif
    static const int KING_VAL = 2500;
    
    // stolen from SebLague
    static int pawn_squares[64];
		static int knight_squares[64];
		static int bishop_squares[64];
		static int rook_squares[64];
		static int queen_squares[64];
		static int king_squares[64];

};



#include "engine/engine.h"
#include "debug.h"

#include <chrono>
#include <list>
#include <vector>
#include <limits>
#include "engine/moveordering.h"
#include "game/square.h"
#include "game/board.h"
#include "pieceID.h"

#ifdef DEBUG
  int nodes_searched = 0;
  int branches_pruned = 0;
#endif

Version Engine::version = Version("Vengence", 'b', 1, 1, 0);

int Engine::pawn_squares[64] = {
	0,  0,  0,  0,  0,  0,  0,  0,
	50, 50, 50, 50, 50, 50, 50, 50,
	10, 10, 20, 30, 30, 20, 10, 10,
	5,  5, 10, 25, 25, 10,  5,  5,
	0,  0,  0, 20, 20,  0,  0,  0,
	5, -5,-10,  0,  0,-10, -5,  5,
	5, 10, 10,-20,-20, 10, 10,  5,
	0,  0,  0,  0,  0,  0,  0,  0
};

int Engine::knight_squares[64] = {
	-50,-40,-30,-30,-30,-30,-40,-50,
	-40,-20,  0,  0,  0,  0,-20,-40,
	-30,  0, 10, 15, 15, 10,  0,-30,
	-30,  5, 15, 20, 20, 15,  5,-30,
	-30,  0, 15, 20, 20, 15,  0,-30,
	-30,  5, 10, 15, 15, 10,  5,-30,
	-40,-20,  0,  5,  5,  0,-20,-40,
	-50,-40,-30,-30,-30,-30,-40,-50,
};


int Engine::bishop_squares[64] = {
	-20,-10,-10,-10,-10,-10,-10,-20,
	-10,  0,  0,  0,  0,  0,  0,-10,
	-10,  0,  5, 10, 10,  5,  0,-10,
	-10,  5,  5, 10, 10,  5,  5,-10,
	-10,  0, 10, 10, 10, 10,  0,-10,
	-10, 10, 10, 10, 10, 10, 10,-10,
	-10,  5,  0,  0,  0,  0,  5,-10,
	-20,-10,-10,-10,-10,-10,-10,-20,
};

int Engine::rook_squares[64] ={
	0,  0,  0,  0,  0,  0,  0,  0,
	5, 10, 10, 10, 10, 10, 10,  5,
	-5,  0,  0,  0,  0,  0,  0, -5,
	-5,  0,  0,  0,  0,  0,  0, -5,
	-5,  0,  0,  0,  0,  0,  0, -5,
	-5,  0,  0,  0,  0,  0,  0, -5,
	-5,  0,  0,  0,  0,  0,  0, -5,
	0,  0,  0,  5,  5,  0,  0,  0
};
int Engine::queen_squares[64] = {
	-20,-10,-10, -5, -5,-10,-10,-20,
	-10,  0,  0,  0,  0,  0,  0,-10,
	-10,  0,  5,  5,  5,  5,  0,-10,
	-5,  0,  5,  5,  5,  5,  0, -5,
	0,  0,  5,  5,  5,  5,  0, -5,
	-10,  5,  5,  5,  5,  5,  0,-10,
	-10,  0,  5,  0,  0,  0,  0,-10,
	-20,-10,-10, -5, -5,-10,-10,-20
};
int Engine::king_squares[64] = {
	-30,-40,-40,-50,-50,-40,-40,-30,
	-30,-40,-40,-50,-50,-40,-40,-30,
	-30,-40,-40,-50,-50,-40,-40,-30,
	-30,-40,-40,-50,-50,-40,-40,-30,
	-20,-30,-30,-40,-40,-30,-30,-20,
	-10,-20,-20,-20,-20,-20,-20,-10,
	20, 20,  0,  0,  0,  0, 20, 20,
	20, 30, 10,  0,  0, 10, 30, 20
};

SearchResult Engine::alpha_beta(const Board& board, int depth, int alpha, int beta)
{
  if(clock_res::now() > endtime)
    abort = true;
  #ifdef DEBUG
    nodes_searched ++;
  #endif
  std::vector<Move> moves = board.getValidMoves();
  
  GameResult gr = board.getGameResult(false);
  if(depth == 0 || gr != ONGOING)
  {
    //if(clock_res::now() > endtime)
    //  abort = true;
    return SearchResult(static_eval(board), gr, 0);
  }
  if(moves.empty())
  {
    //if(clock_res::now() > endtime)
    //  abort = true;
      
    return SearchResult(-100, STALEMATE, 0);
  }
  
  if(!pv.empty())
  {
    MoveOrdering mo(pv.back());
    pv.pop_back();
    mo.order(board, moves);
  }
  
  SearchResult best = SearchResult(Engine::neg_inf-1, ONGOING, depth);
  
  

  for(std::vector<Move>::iterator i = moves.begin(); i != moves.end(); i++)
  {
    
    if(abort)
      break;
    
    Board next = board.makeMove(*i, true);
    SearchResult sr = alpha_beta(next, depth - 1, -beta, -alpha);
    
    if(sr.result != STALEMATE)
      sr.score *= -1;
    
    if(sr.score > best.score)
    {
      best = sr;
      best.depth++;
      best.pv.push_back(*i);
    }
    
    if(sr.score > alpha)
    {
      alpha = sr.score;
    }
    
    if(alpha >= beta)
    {
      #ifdef DEBUG
        branches_pruned++;
      #endif
      break;
    }
  }
  
  if(abort)
  {
    return SearchResult(0,ABORT,0);
  }
  
  return best;
}

SearchResult Engine::solve(const Board& board, const Time endtime, const int maxDepth = 100)
{
  abort = false;
  this->endtime = endtime;

  SearchResult res(0, ABORT, 0);
  int depth = 1;
  while(!abort)
  {
    #ifdef DEBUG
    try
    {
        nodes_searched = 0;
        branches_pruned = 0;
      
        std::cout << "Ply " << depth << ": ";
    #endif
    
      SearchResult sr = alpha_beta(board, depth++, Engine::neg_inf, Engine::pos_inf);
      
      #ifdef DEBUG
        std::cout << "Searched " << nodes_searched << ", ";
        std::cout << "Pruned " << branches_pruned << ". ";
        std::cout << (sr.result == ABORT ? "ABORTED " : "") << " ";
      #endif
    
      if(sr.result != ABORT)
      {
        res = sr;
        pv = sr.pv;
        #ifdef DEBUG
          std::cout << "BM: " << sr.pv.back().toString() << ", score: " << sr.score;
        #endif
      }
      #ifdef DEBUG
        std::cout << std::endl;
      #endif
      if(res.result != ONGOING)
        break;
      if(depth-1 >= maxDepth)
        break;
    #ifdef DEBUG
    }
    catch(const std::bad_alloc& e)
    {
      std::cerr << "bad_alloc caught: "<< e.what() << std::endl;
      break;
    }
    #endif
  }
  
  if(res.pv.size() > 0)
    res.bm = res.pv.back();
  return res;
}
SearchResult Engine::solve(const Board& board, const float seconds)
{
  return solve(board, clock_res::now() + std::chrono::milliseconds((int)(seconds*1000)));
}
std::vector<MoveScore> Engine::rankMoves(const Board& board, const float maxSeconds, const int maxDepth)
{
  std::vector<Move> moves = board.getValidMoves();
  if(moves.size() == 0)
    return std::vector<MoveScore>();
  
  std::vector<Board> nextBoards;
  
  for(std::vector<Move>::iterator i = moves.begin(); i != moves.end(); i++)
  {
    nextBoards.push_back(board.makeMove(*i, true));
  }
  
  std::vector<MoveScore> ms_list(nextBoards.size());
  
  Time endTime = clock_res::now() + std::chrono::milliseconds((int)(maxSeconds*1000));
  
  bool foundWinningMove = false;
  for(int depth = 1; depth <= maxDepth; depth++)
  {
    int i = 0;
    for(std::vector<Board>::iterator b = nextBoards.begin(); b != nextBoards.end(); b++)
    {
      Move move = moves[i];
      #ifdef DEBUG
        std::cout << move.toString() << std::endl;
      #endif
      GameResult gr = b->getGameResult();
      //SearchResult sr(Engine::static_eval(*b), gr, 0); 
      if(gr == ONGOING)
      {
        //SearchResult depthSearch = solve(*b, endTime, depth);
        SearchResult sr = solve(*b, endTime, depth);
        if(sr.result != ABORT)
        {
          sr.score *= -1;
          ms_list[i] = MoveScore(move, sr);
          
          if(sr.score == Engine::pos_inf)
            foundWinningMove = true;
        }
      }
      else if(gr == WHITE_VICTORY || gr == BLACK_VICTORY)
      {
        SearchResult sr(-Engine::static_eval(*b), gr, 1);
        ms_list[i] = MoveScore(move, sr);
        foundWinningMove = true;
      }
      
      //if(abort)
      //  return ms_list;
      
      i++;
    }
  
    if(clock_res::now() > endtime || foundWinningMove)
      break;
  }
  
  return ms_list;
}


int Engine::static_eval(const Board& board) const
{
  switch(board.getGameResult(false))
  {
    case WHITE_VICTORY:
      if(board.turn == WHITE)
        return Engine::pos_inf;
      else return Engine::neg_inf;
    case BLACK_VICTORY:
      if(board.turn == BLACK)
        return Engine::pos_inf;
      else return Engine::neg_inf;
    case STALEMATE:
      return -100;
    default: break;
  }
  
  int sum = 0;
  for(int i = 0; i < 16; i++) // white pieces
  {
    if(board.pieces[i].isOnBoard)
      sum += eval_piece(board, board.pieces[i]);
  }
  for(int i = 16; i < 32; i++) // black pieces
  {
    if(board.pieces[i].isOnBoard)
      sum -= eval_piece(board, board.pieces[i]);
  }
  
  // value is in terms of who's being evaled
  sum *= (board.turn == WHITE ? 1:-1);
  
  // TODO tune
  sum -= board.halfMoveClock*board.halfMoveClock;
  
  return sum;
}

// TODO make it good
int Engine::eval_piece(const Board& board, const Piece piece) const
{
  if(!piece.isOnBoard)
    return 0;
  
  unsigned char table_index = piece.current.toIndex();
  
  if(piece.color == WHITE)
  {
    table_index = Square(piece.current.file, 7-piece.current.rank).toIndex();
  }
  
  //Square opp_king_sqr = board.pieces[idFromStats((PlayerColor)!piece.color, KING, E)].current;
  
  int val = 0;
  switch(piece.type)
  {
    case PAWN:
      val =  PAWN_VAL;
      val += Engine::pawn_squares[table_index];
    break;
    
    case BISHOP:
      val = BISHOP_VAL;
      val += Engine::bishop_squares[table_index];
    break;
    case ROOK:
      val = ROOK_VAL;
      val += Engine::rook_squares[table_index];
    break;
    case KNIGHT:
      val = KNIGHT_VAL;
      val += Engine::knight_squares[table_index];
    break;
    case QUEEN:
      val = QUEEN_VAL;
      val += Engine::queen_squares[table_index];
    break;
    case KING:
      val = KING_VAL;
      val += Engine::king_squares[table_index];
    break;
    default: break;
  }
  return val;
}

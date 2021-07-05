#include <emscripten/bind.h>
#include <vector>
#include <string>
#include "game/square.h"
#include "game/board.h"
#include "enum.h"
#include "version.h"
#include "pieceID.h"
#include "game/piece.h"
#include "game/game.h"
#include "engine/engine.h"
#include "game/clock.h"

using namespace emscripten;

Version core_v(){return Game::version;}
Version engine_v(){return Engine::version;}

EMSCRIPTEN_BINDINGS(mod)
{
  value_object<Version>("Version")
    .field("name", &Version::name)
    .field("stage", &Version::stage)
    .field("major", &Version::major)
    .field("minor", &Version::minor)
    .field("patch", &Version::patch)
  ;
  
  function("colorOfPiece", &colorOfPiece);
  function("typeOfPiece", &typeOfPiece);
  function("homeBoardSideOfPiece", &homeBoardSideOfPiece);
  function("homeFileOfPiece", &homeFileOfPiece);
  function("homeOfPiece", &homeOfPiece);
  function("idFromStats", &idFromStats);
  
  class_<Piece>("Piece")
    .constructor<>()
    .constructor<PlayerColor, PieceType, char, char>()
    .constructor<PieceIdentifier>()
    .property("isOnBoard", &Piece::isOnBoard)
    .property("hasMoved", &Piece::hasMoved)
    .property("color", &Piece::color)
    .property("type", &Piece::type)
    .property("numCaptures", &Piece::numCaptures)
    .property("home", &Piece::home)
    .property("current", &Piece::current)
    .function("getCapture", &Piece::getCapture)
  ;
  
  class_<Square>("Square")
    .constructor<unsigned char>()
    .constructor<unsigned char, unsigned char>()
    .property("file", &Square::file)
    .property("rank", &Square::rank)
    .function("toIndex", &Square::toIndex)
    .function("translate", &Square::translate)
    .function("isValid", &Square::isValid)
    .function("toString", &Square::toString)
    .function("dist", &Square::dist)
    .function("eq", &Square::eq)
  ;
  
  class_<Move>("Move")
    .constructor<Square, Square, PieceType>()
    .constructor<Square, Square>()
    .property("from", &Move::from)
    .property("to", &Move::to)
    .property("promotion", &Move::promotion)
    .function("toString", &Move::toString)
    .function("eq", &Move::eq)
  ;

  class_<Board>("Board")
    .constructor<>()
    .constructor<std::string>()
    .function("serialize", &Board::serialize)
    .property("turn", &Board::turn)
    .property("pawnEnPassantFile", &Board::pawnEnPassantFile)
    .property("halfMoveClock", &Board::halfMoveClock)
    .function("getPlayField", &Board::getPlayField)
    .function("getBoardPiece", &Board::getBoardPiece)
    .function("getValidMoves", select_overload<std::vector<Move>(void) const>(&Board::getValidMoves))
    .function("getValidMoves", select_overload<std::vector<Move>(PlayerColor) const>(&Board::getValidMoves))
    .function("getValidMoves", select_overload<std::vector<Move>(PieceIdentifier) const>(&Board::getValidMoves))
    .function("makeMove", &Board::makeMove)
    .function("getGameResult", &Board::getGameResult)
    .function("hasKing", &Board::hasKing)
  ;
  
  enum_<PlayerColor>("PlayerColor")
    .value("BLACK", BLACK)
    .value("WHITE", WHITE)
  ;
  
  enum_<PieceType>("PieceType")
    .value("PAWN",PAWN)
    .value("ROOK",ROOK)
    .value("KNIGHT",KNIGHT)
    .value("BISHOP", BISHOP)
    .value("KING",KING)
    .value("QUEEN",QUEEN)
    .value("NO",NO)
  ;

  enum_<BoardSide>("BoardSide")
    .value("KINGSIDE",KINGSIDE)
    .value("QUEENSIDE",QUEENSIDE)
  ;

  enum_<File>("FILE")
    .value("NONE",NONE)
    .value("A",A)
    .value("B",B)
    .value("C",C)
    .value("D",D)
    .value("E",E)
    .value("F",F)
    .value("G",G)
    .value("H",H)
  ;

  enum_<PieceIdentifier>("PieceIdentifier")
    .value("WHITE_QUEENSIDE_ROOK", WHITE_QUEENSIDE_ROOK)
    .value("WHITE_QUEENSIDE_KNIGHT", WHITE_QUEENSIDE_KNIGHT)
    .value("WHITE_QUEENSIDE_BISHOP", WHITE_QUEENSIDE_BISHOP)
    .value("WHITE_QUEEN", WHITE_QUEEN)
    .value("WHITE_KINGSIDE_ROOK", WHITE_KINGSIDE_ROOK)
    .value("WHITE_KINGSIDE_KNIGHT", WHITE_KINGSIDE_KNIGHT)
    .value("WHITE_KINGSIDE_BISHOP", WHITE_KINGSIDE_BISHOP)
    .value("WHITE_KING", WHITE_KING)
    .value("WHITE_A_PAWN", WHITE_A_PAWN)
    .value("WHITE_B_PAWN", WHITE_B_PAWN)
    .value("WHITE_C_PAWN", WHITE_C_PAWN)
    .value("WHITE_D_PAWN", WHITE_D_PAWN)
    .value("WHITE_E_PAWN", WHITE_E_PAWN)
    .value("WHITE_F_PAWN", WHITE_F_PAWN)
    .value("WHITE_G_PAWN", WHITE_G_PAWN)
    .value("WHITE_H_PAWN", WHITE_H_PAWN)
    
    .value("BLACK_QUEENSIDE_ROOK", BLACK_QUEENSIDE_ROOK)
    .value("BLACK_QUEENSIDE_KNIGHT", BLACK_QUEENSIDE_KNIGHT)
    .value("BLACK_QUEENSIDE_BISHOP", BLACK_QUEENSIDE_BISHOP)
    .value("BLACK_QUEEN", BLACK_QUEEN)
    .value("BLACK_KINGSIDE_ROOK", BLACK_KINGSIDE_ROOK)
    .value("BLACK_KINGSIDE_KNIGHT", BLACK_KINGSIDE_KNIGHT)
    .value("BLACK_KINGSIDE_BISHOP", BLACK_KINGSIDE_BISHOP)
    .value("BLACK_KING", BLACK_KING)
    .value("BLACK_A_PAWN", BLACK_A_PAWN)
    .value("BLACK_B_PAWN", BLACK_B_PAWN)
    .value("BLACK_C_PAWN", BLACK_C_PAWN)
    .value("BLACK_D_PAWN", BLACK_D_PAWN)
    .value("BLACK_E_PAWN", BLACK_E_PAWN)
    .value("BLACK_F_PAWN", BLACK_F_PAWN)
    .value("BLACK_G_PAWN", BLACK_G_PAWN)
    .value("BLACK_H_PAWN", BLACK_H_PAWN)
    
    .value("EMPTY", EMPTY)
  ;

  enum_<GameResult>("GameResult")
    .value("WHITE_VICTORY", WHITE_VICTORY)
    .value("BLACK_VICTORY", BLACK_VICTORY)
    .value("STALEMATE", STALEMATE)
    .value("ONGOING", ONGOING)
    
    .value("ABORT", ABORT)
  ;
  
  function("core_v", &core_v);
  function("engine_v", &engine_v);
  
  class_<Game>("Game")
    .constructor<>()
    .property("board", &Game::board)
    .property("prevBoards", &Game::prevBoards)
    .property("clock", &Game::clock)
    .function("takeBack", &Game::takeBack)
    .function("commitMove", &Game::commitMove)
  ;
  
  class_<Clock>("Clock")
    .constructor<>()
    .constructor<unsigned int, unsigned int, IncrementMethod>()
    .constructor<unsigned int, unsigned int, IncrementMethod, unsigned int, unsigned int, IncrementMethod>()
    .function("toggle", &Clock::toggle)
    .function("stop", &Clock::stop)
    .function("getBlackTime", &Clock::getBlackTime)
    .function("getWhiteTime", &Clock::getWhiteTime)
    .function("isWhiteRunning", &Clock::isWhiteRunning)
    .function("isBlackRunning", &Clock::isBlackRunning)
    .function("getDelayLeft", &Clock::getDelayLeft)
    .function("getResultFromFlag", &Clock::getResultFromFlag)
  ;
  
  enum_<IncrementMethod>("IncrementMethod")
    .value("INCREMENT", INCREMENT)
    .value("DELAY", DELAY)
    .value("BRONSTEIN", BRONSTEIN)
  ;
  
  value_object<SearchResult>("SearchResult")
    .field("score", &SearchResult::score)
    .field("result", &SearchResult::result)
    .field("depth", &SearchResult::depth)
    .field("pv", &SearchResult::pv)
    .field("bm", &SearchResult::bm)
  ;
  
  value_object<MoveScore>("MoveScore")
    .field("move", &MoveScore::move)
    .field("score", &MoveScore::score)
  ;
  
  class_<Engine>("Engine")
    .constructor<>()
    .function("solve", select_overload<SearchResult(const Board&, const float)>(&Engine::solve))
    .function("rankMoves", &Engine::rankMoves)
    .function("static_eval", &Engine::static_eval)
    .property("abort", &Engine::abort)
  ;
  
  register_vector<Move>("vector<Move>");
  register_vector<Board>("vector<Board>");
  register_vector<MoveScore>("vector<MoveScore>");
}

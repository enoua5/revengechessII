#include "debug.h" 

#include "game/board.h"
#include "game/square.h"
#include "enum.h"
#include "pieceID.h"
#include <vector>
#include <queue>
#include <iterator>
#include <exception>

//#ifdef __EMSCRIPTEN__
  #include "nlohmann/json.hpp"
//#endif

Board::Board()
{
  turn = WHITE;
  pawnEnPassantFile = NONE;
  halfMoveClock = 0;
  
  PieceIdentifier startPos[64] = {
    //r1
    WHITE_QUEENSIDE_ROOK,
    WHITE_QUEENSIDE_KNIGHT,
    WHITE_QUEENSIDE_BISHOP,
    WHITE_QUEEN,
    WHITE_KING,
    WHITE_KINGSIDE_BISHOP,
    WHITE_KINGSIDE_KNIGHT,
    WHITE_KINGSIDE_ROOK,
    //r2
    WHITE_A_PAWN,
    WHITE_B_PAWN,
    WHITE_C_PAWN,
    WHITE_D_PAWN,
    WHITE_E_PAWN,
    WHITE_F_PAWN,
    WHITE_G_PAWN,
    WHITE_H_PAWN,
    //r3
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    //r4
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    //r5
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    //r6
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    //r7
    BLACK_A_PAWN,
    BLACK_B_PAWN,
    BLACK_C_PAWN,
    BLACK_D_PAWN,
    BLACK_E_PAWN,
    BLACK_F_PAWN,
    BLACK_G_PAWN,
    BLACK_H_PAWN,
    //r8
    BLACK_QUEENSIDE_ROOK,
    BLACK_QUEENSIDE_KNIGHT,
    BLACK_QUEENSIDE_BISHOP,
    BLACK_QUEEN,
    BLACK_KING,
    BLACK_KINGSIDE_BISHOP,
    BLACK_KINGSIDE_KNIGHT,
    BLACK_KINGSIDE_ROOK
  };
  
  for(int i = 0; i < 64; i ++)
    playField[i]=startPos[i];
    
  pieces[WHITE_QUEENSIDE_ROOK] = Piece(WHITE, ROOK, 0, 0);
  pieces[WHITE_QUEENSIDE_KNIGHT] = Piece(WHITE, KNIGHT, 1, 0);
  pieces[WHITE_QUEENSIDE_BISHOP] = Piece(WHITE, BISHOP, 2, 0);
  pieces[WHITE_QUEEN] = Piece(WHITE, QUEEN, 3, 0);
  pieces[WHITE_KING] = Piece(WHITE, KING, 4, 0);
  pieces[WHITE_KINGSIDE_BISHOP] = Piece(WHITE, BISHOP, 5, 0);
  pieces[WHITE_KINGSIDE_KNIGHT] = Piece(WHITE, KNIGHT, 6, 0);
  pieces[WHITE_KINGSIDE_ROOK] = Piece(WHITE, ROOK, 7, 0);
  
  pieces[WHITE_A_PAWN] = Piece(WHITE, PAWN, 0, 1);
  pieces[WHITE_B_PAWN] = Piece(WHITE, PAWN, 1, 1);
  pieces[WHITE_C_PAWN] = Piece(WHITE, PAWN, 2, 1);
  pieces[WHITE_D_PAWN] = Piece(WHITE, PAWN, 3, 1);
  pieces[WHITE_E_PAWN] = Piece(WHITE, PAWN, 4, 1);
  pieces[WHITE_F_PAWN] = Piece(WHITE, PAWN, 5, 1);
  pieces[WHITE_G_PAWN] = Piece(WHITE, PAWN, 6, 1);
  pieces[WHITE_H_PAWN] = Piece(WHITE, PAWN, 7, 1);
  
  pieces[BLACK_QUEENSIDE_ROOK] = Piece(BLACK, ROOK, 0, 7);
  pieces[BLACK_QUEENSIDE_KNIGHT] = Piece(BLACK, KNIGHT, 1, 7);
  pieces[BLACK_QUEENSIDE_BISHOP] = Piece(BLACK, BISHOP, 2, 7);
  pieces[BLACK_QUEEN] = Piece(BLACK, QUEEN, 3, 7);
  pieces[BLACK_KING] = Piece(BLACK, KING, 4, 7);
  pieces[BLACK_KINGSIDE_BISHOP] = Piece(BLACK, BISHOP, 5, 7);
  pieces[BLACK_KINGSIDE_KNIGHT] = Piece(BLACK, KNIGHT, 6, 7);
  pieces[BLACK_KINGSIDE_ROOK] = Piece(BLACK, ROOK, 7, 7);
  
  pieces[BLACK_A_PAWN] = Piece(BLACK, PAWN, 0, 6);
  pieces[BLACK_B_PAWN] = Piece(BLACK, PAWN, 1, 6);
  pieces[BLACK_C_PAWN] = Piece(BLACK, PAWN, 2, 6);
  pieces[BLACK_D_PAWN] = Piece(BLACK, PAWN, 3, 6);
  pieces[BLACK_E_PAWN] = Piece(BLACK, PAWN, 4, 6);
  pieces[BLACK_F_PAWN] = Piece(BLACK, PAWN, 5, 6);
  pieces[BLACK_G_PAWN] = Piece(BLACK, PAWN, 6, 6);
  pieces[BLACK_H_PAWN] = Piece(BLACK, PAWN, 7, 6);
}

Board::Board(const Board* other)
{
  turn = other->turn;
  pawnEnPassantFile = other->pawnEnPassantFile;
  halfMoveClock = other->halfMoveClock;
  for(int i = 0; i < 64; i++)
    playField[i] = other->playField[i];
  for(int i = 0; i < 32; i++)
    pieces[i] = other->pieces[i];
}

//#ifdef __EMSCRIPTEN__
  std::string Board::serialize()
  {
    /*
      Why did I serialize this manually?
      - Because I hadn't downloaded the JSON library yet
        when I wrote this part.
      
      Why didn't I update it after?
      - Laziness.
      
      Feel free to fix up this function, but for now it works.
     */
    std::string json = "{\"turn\": ";
    json += (turn == WHITE) ? "true" : "false";
    json += ", \"pawnEnPassantFile\": ";
    std::string files[] = {"\"A\"",
                           "\"B\"",
                           "\"C\"",
                           "\"D\"",
                           "\"E\"",
                           "\"F\"",
                           "\"G\"",
                           "\"H\"",
                           "\"X\""};
    json += files[pawnEnPassantFile];
    json += ", \"halfMoveClock\": " + std::to_string((int)halfMoveClock);

    json += ", \"pieces\": [";
    for(int i = 0; i < 32; i++)
    {
      auto p = pieces[i];
      
      if(i != 0)
        json += ", ";
      json += "{";
     
      json += "\"isOnBoard\": ";
      json += p.isOnBoard ? "true" : "false";
      json += ", \"hasMoved\": ";
      json += p.hasMoved ? "true" : "false";
      json += ", \"color\": ";
      json += (p.color == WHITE) ? "true" : "false";
      json += ", \"type\": ";
      
      std::string pieceTypes[] = {
        "\"PAWN\"",
        "\"ROOK\"",
        "\"KNIGHT\"",
        "\"BISHOP\"",
        "\"KING\"",
        "\"QUEEN\"",
        "\"NO\"",
      };
      json += pieceTypes[p.type];
    
      json += ", \"home\": ";
      json += "\"" + p.home.toString() + "\"";
      json += ", \"current\": ";
      json += "\"" + p.current.toString() + "\"";
      
      json += ", \"captures\": [";
      std::string ids[] = {
        "\"WHITE_QUEENSIDE_ROOK\"",
        "\"WHITE_QUEENSIDE_KNIGHT\"",
        "\"WHITE_QUEENSIDE_BISHOP\"",
        "\"WHITE_QUEEN\"",
        "\"WHITE_KINGSIDE_ROOK\"",
        "\"WHITE_KINGSIDE_KNIGHT\"",
        "\"WHITE_KINGSIDE_BISHOP\"",
        "\"WHITE_KING\"",
        "\"WHITE_A_PAWN\"",
        "\"WHITE_B_PAWN\"",
        "\"WHITE_C_PAWN\"",
        "\"WHITE_D_PAWN\"",
        "\"WHITE_E_PAWN\"",
        "\"WHITE_F_PAWN\"",
        "\"WHITE_G_PAWN\"",
        "\"WHITE_H_PAWN\"",
        
        "\"BLACK_QUEENSIDE_ROOK\"",
        "\"BLACK_QUEENSIDE_KNIGHT\"",
        "\"BLACK_QUEENSIDE_BISHOP\"",
        "\"BLACK_QUEEN\"",
        "\"BLACK_KINGSIDE_ROOK\"",
        "\"BLACK_KINGSIDE_KNIGHT\"",
        "\"BLACK_KINGSIDE_BISHOP\"",
        "\"BLACK_KING\"",
        "\"BLACK_A_PAWN\"",
        "\"BLACK_B_PAWN\"",
        "\"BLACK_C_PAWN\"",
        "\"BLACK_D_PAWN\"",
        "\"BLACK_E_PAWN\"",
        "\"BLACK_F_PAWN\"",
        "\"BLACK_G_PAWN\"",
        "\"BLACK_H_PAWN\"",
        
        "\"EMPTY\""
      };
      for(int c = 0; c < p.numCaptures; c++)
      {
        if(c != 0)
          json += ", ";
        json += ids[p.captures[c]];
      }
      json += "]";
      
      json += "}"; 
      
    }
    json += "]";
    
    json += "}";
    return json;
  }
  Square squareFromString(std::string coord)
  {
    // more-or-less copied from ui.cpp
    // TODO may be good to refactor this into shared code
    unsigned char file = coord[0];
    unsigned char rank = coord[1];
    
    file |= 0x20; // to lowercase
    // to 0-7 range
    file -= 'a';
    rank -= '1';
    
    return Square(file, rank);
  }
  Board::Board(std::string data) // deserialize
  {
    // init the play field. This will be filled in later
    // when parsing the individual pieces.
    for(int i = 0; i < 64; i++)
      playField[i] = EMPTY;
      
    nlohmann::json parsed = nlohmann::json::parse(data);
    
    turn = parsed.value("turn", true) ? WHITE : BLACK;
    switch(parsed.value("pawnEnPassantFile", "X")[0])
    {
      case 'A':
        pawnEnPassantFile = A;
        break;
      case 'B':
        pawnEnPassantFile = B;
        break;
      case 'C':
        pawnEnPassantFile = C;
        break;
      case 'D':
        pawnEnPassantFile = D;
        break;
      case 'E':
        pawnEnPassantFile = E;
        break;
      case 'F':
        pawnEnPassantFile = F;
        break;
      case 'G':
        pawnEnPassantFile = G;
        break;
      case 'H':
        pawnEnPassantFile = H;
        break;
      default:
        pawnEnPassantFile = NONE;
    }
    halfMoveClock = parsed.value("halfMoveClock", 0);
    
    
    
    nlohmann::json piece_list = parsed.value("pieces", nlohmann::json::array());
    
    int i = 0;
    for(auto& parsed_piece : piece_list)
    {
      if(i >= 32)
        break;
      
      pieces[i].color = parsed_piece.value("color", true) ? WHITE : BLACK;
      pieces[i].hasMoved = parsed_piece.value("hasMoved", false);
      
      std::string typeStr = parsed_piece.value("type", "NO");
      if(typeStr == "PAWN")
        pieces[i].type = PAWN;
      else if(typeStr == "ROOK")
        pieces[i].type = ROOK;
      else if(typeStr == "KNIGHT")
        pieces[i].type = KNIGHT;
      else if(typeStr == "BISHOP")
        pieces[i].type = BISHOP;
      else if(typeStr == "KING")
        pieces[i].type = KING;
      else if(typeStr == "QUEEN")
        pieces[i].type = QUEEN;
      else
        pieces[i].type = NO;
      
      // this will be updated further on
      pieces[i].numCaptures = 0;
      
      pieces[i].home = squareFromString(parsed_piece.value("home", "A1"));
      
      bool isOnBoard = parsed_piece.value("isOnBoard", false);
      pieces[i].isOnBoard = isOnBoard;
      if(isOnBoard)
      {
        Square current = squareFromString(parsed_piece.value("current", "A1"));
        pieces[i].current = current;
        
        playField[current.toIndex()] = (PieceIdentifier)i;
        
        nlohmann::json capture_list = parsed_piece.value("captures", nlohmann::json::array());
        int numCaptures = 0;
        for(auto& captured : capture_list)
        {
          if(numCaptures >= 17)
            break;
          
          PieceIdentifier capID = EMPTY;
          if(captured == "WHITE_QUEENSIDE_ROOK")
            capID = WHITE_QUEENSIDE_ROOK;
          else if(captured == "WHITE_QUEENSIDE_KNIGHT")
            capID = WHITE_QUEENSIDE_KNIGHT;
          else if(captured == "WHITE_QUEENSIDE_BISHOP")
            capID = WHITE_QUEENSIDE_BISHOP;
          else if(captured == "WHITE_QUEEN")
            capID = WHITE_QUEEN;
          else if(captured == "WHITE_KINGSIDE_ROOK")
            capID = WHITE_KINGSIDE_ROOK;
          else if(captured == "WHITE_KINGSIDE_KNIGHT")
            capID = WHITE_KINGSIDE_KNIGHT;
          else if(captured == "WHITE_KINGSIDE_BISHOP")
            capID = WHITE_KINGSIDE_BISHOP;
          else if(captured == "WHITE_KING")
            capID = WHITE_KING;
          else if(captured == "WHITE_A_PAWN")
            capID = WHITE_A_PAWN;
          else if(captured == "WHITE_B_PAWN")
            capID = WHITE_B_PAWN;
          else if(captured == "WHITE_C_PAWN")
            capID = WHITE_C_PAWN;
          else if(captured == "WHITE_D_PAWN")
            capID = WHITE_D_PAWN;
          else if(captured == "WHITE_E_PAWN")
            capID = WHITE_E_PAWN;
          else if(captured == "WHITE_F_PAWN")
            capID = WHITE_F_PAWN;
          else if(captured == "WHITE_G_PAWN")
            capID = WHITE_G_PAWN;
          else if(captured == "WHITE_H_PAWN")
            capID = WHITE_H_PAWN;
          else if(captured == "BLACK_QUEENSIDE_ROOK")
            capID = BLACK_QUEENSIDE_ROOK;
          else if(captured == "BLACK_QUEENSIDE_KNIGHT")
            capID = BLACK_QUEENSIDE_KNIGHT;
          else if(captured == "BLACK_QUEENSIDE_BISHOP")
            capID = BLACK_QUEENSIDE_BISHOP;
          else if(captured == "BLACK_QUEEN")
            capID = BLACK_QUEEN;
          else if(captured == "BLACK_KINGSIDE_ROOK")
            capID = BLACK_KINGSIDE_ROOK;
          else if(captured == "BLACK_KINGSIDE_KNIGHT")
            capID = BLACK_KINGSIDE_KNIGHT;
          else if(captured == "BLACK_KINGSIDE_BISHOP")
            capID = BLACK_KINGSIDE_BISHOP;
          else if(captured == "BLACK_KING")
            capID = BLACK_KING;
          else if(captured == "BLACK_A_PAWN")
            capID = BLACK_A_PAWN;
          else if(captured == "BLACK_B_PAWN")
            capID = BLACK_B_PAWN;
          else if(captured == "BLACK_C_PAWN")
            capID = BLACK_C_PAWN;
          else if(captured == "BLACK_D_PAWN")
            capID = BLACK_D_PAWN;
          else if(captured == "BLACK_E_PAWN")
            capID = BLACK_E_PAWN;
          else if(captured == "BLACK_F_PAWN")
            capID = BLACK_F_PAWN;
          else if(captured == "BLACK_G_PAWN")
            capID = BLACK_G_PAWN;
          else if(captured == "BLACK_H_PAWN")
            capID = BLACK_H_PAWN;
          
          pieces[i].captures[numCaptures] = capID;
          
          numCaptures++;
        }
        pieces[i].numCaptures = numCaptures;
      }
    
      i++;
    }
  }
//#endif

std::vector<Move> Board::getValidMoves(PieceIdentifier id) const
{
  std::vector<Move> moves;
  
  Piece p = this->pieces[id];
  if(!p.isOnBoard)
    return moves; // no moves for a captured piece
  
  switch(p.type)
  {
    case PAWN:
      addPawnMoves(moves, p);
    break;
    case ROOK:
      addRookMoves(moves, p);
    break;
    case KNIGHT:
      addKnightMoves(moves, p);
    break;
    case BISHOP:
      addBishopMoves(moves, p);
    break;
    case QUEEN:
      addQueenMoves(moves, p);
    break;
    case KING:
      addKingMoves(moves, p);
    break;
    
    default: break;
  }
  return moves;
}

PieceIdentifier Board::getPlayField(unsigned char i)
{
  if(i >= 64)
    return EMPTY;
  return playField[i];
}
Piece Board::getBoardPiece(unsigned char i)
{
  if(i >= 32)
    return Piece();
  return pieces[i];
}

std::vector<Move> Board::getValidMoves() const
{
  return getValidMoves(this->turn);
}

std::vector<Move> Board::getValidMoves(PlayerColor c) const
{
  std::vector<Move> moves;
  
  if(getGameResult(false) != ONGOING)
    return moves;
  
  int start = 0;
  int end = 16;
  
  if(c == BLACK)
  {
    start = 16;
    end = 32;
  }
    
  
  for(int i = start; i < end; i++) // for each piece
  {
    // add that piece's moves
    std::vector<Move> newMoves = getValidMoves((PieceIdentifier) i);
    moves.insert(moves.end(), newMoves.begin(), newMoves.end());
  }
  
  return moves;
}
int valgrind_test_please_remove = 0;
Board Board::makeMove(const Move move, bool trusted) const
{
  Board next(this);
  
  // if we didn't already check the validity, do that now
  if(!trusted)
  {
    auto v = getValidMoves();
    bool valid = false;
    
    std::vector<Move> :: iterator i;
    for(i = v.begin(); i != v.end(); i++)
    {
      if(move == *i)
      {
        valid = true;
        break;
      }
    }
    
    if(!valid)
      throw invalid_move();
  }
  
  // increment the 75-move rule clock
  next.halfMoveClock++;
  
  // save these to reduce recalcs
  PieceIdentifier attacking = this->playField[move.from.toIndex()];
  PieceIdentifier defending = this->playField[move.to.toIndex()];
  
  #ifdef DEBUG
    if(attacking == EMPTY)
    {
       std::cerr << "Attempt to move EMPTY. Aborting." << std::endl;
       std::cerr << "Illegal move was: " << move.toString() << std::endl;
       return next;
    }
  #endif
  
  // various features of this are useful
  Piece* mover = &next.pieces[attacking];
  
  // some more precalcs to figure out what's going on
  bool pawnMove = mover->type == PAWN; // ignore promoted pawns
  bool enPassant = pawnMove && (defending == EMPTY) && (move.from.file != move.to.file);
  
  if(pawnMove)
  {
    next.halfMoveClock = 0;
    // if they've made it to the end of the board
    if(move.to.rank == 0 || move.to.rank == 7) mover->type = move.promotion;
    // if they've used the double move
    else if((move.from.rank == 1 && move.to.rank == 3) 
          ||(move.from.rank == 6 && move.to.rank == 4))
            next.pawnEnPassantFile = (File)move.to.file;
    else next.pawnEnPassantFile = NONE;
  }
  else next.pawnEnPassantFile = NONE;
  
  // check if we're castling
  bool castled = false;
  if(typeOfPiece(attacking) == KING && !mover->hasMoved)
  {
    // this is the king's first move
    // ignores promoted pawns
    if(move.to.file == 2)
    {
      // queenside castle
      castled = true;
      // move rook beside king
      next.playField[move.from.toIndex()-1] = next.playField[move.from.toIndex()-4];
      // move king
      next.playField[move.to.toIndex()] = attacking;
      
      // update the rook's position now, because it won't be later, unlike the king's
      next.pieces[next.playField[move.from.toIndex()-1]].current = Square(move.from.toIndex()-1);
      
      next.playField[move.from.toIndex()-4] = EMPTY;
      next.playField[move.from.toIndex()] = EMPTY;
      
    }
    else if(move.to.file == 6)
    {
      // king side castle
      castled = true;
      // move rook beside king
      next.playField[move.from.toIndex()+1] = next.playField[move.from.toIndex()+3];
      // move king
      next.playField[move.to.toIndex()] = attacking;
      
      // update the rook's position now, because it won't be later, unlike the king's
      next.pieces[next.playField[move.from.toIndex()+1]].current = Square(move.from.toIndex()+1);
      
      next.playField[move.from.toIndex()+3] = EMPTY;
      next.playField[move.from.toIndex()] = EMPTY;
    }
  }
  if(!castled)
  {
    next.playField[move.to.toIndex()] = attacking;
    next.playField[move.from.toIndex()] = EMPTY;
  }
  
  
  // update the mover's stats
  mover->hasMoved = true;
  mover->current = move.to;
  
  if(defending != EMPTY
     || enPassant)
  {
    // a capture has happened
    next.halfMoveClock = 0;
    
    PieceIdentifier captured;
    if(!enPassant)
    {
      captured = defending;
    }
    else
    {
      char offset = this->turn == WHITE ? -8 : 8; // up or down the board one row from the mover is captured 
      captured = this->playField[move.to.toIndex()+offset];
      
      next.playField[move.to.toIndex()+offset] = EMPTY;
    }
    
    mover->captures[mover->numCaptures++] = captured;
    next.pieces[captured].isOnBoard = false;
    std::queue<PieceIdentifier> toRespawn;
    for(unsigned char i = 0; i < next.pieces[captured].numCaptures; i++)
      toRespawn.push(next.pieces[captured].captures[i]);
    
    while(!toRespawn.empty())
    {
      PieceIdentifier resp = toRespawn.front();
      //remake it
      next.pieces[resp] = Piece(resp);
      unsigned char respPoint = next.pieces[resp].home.toIndex();
      toRespawn.pop();
      
      PieceIdentifier spawnOver = next.playField[respPoint];
      if(spawnOver != EMPTY)
      {
        next.pieces[resp].captures[next.pieces[resp].numCaptures++] = spawnOver;
        next.pieces[spawnOver].isOnBoard = false;
        
        for(unsigned char i = 0; i < next.pieces[spawnOver].numCaptures; i++)
        {
          toRespawn.push(next.pieces[spawnOver].captures[i]);
        }
      }
      next.playField[respPoint] = resp;
    }
  }
    
  next.turn = (PlayerColor)!next.turn;
  
  #ifdef DEBUG
    if(next.playField[move.to.toIndex()] == EMPTY)
      std::cerr << "DESTINATION SQUARE LEFT EMPTY BY MOVE: " << move.toString() << std::endl;
  #endif
  
  return next;
  
}

bool Board::hasKing(PlayerColor c) const
{
  PieceIdentifier king = WHITE_KING;
  PieceIdentifier aPawn = WHITE_A_PAWN;
  if(c == BLACK)
  {
    king = BLACK_KING;
    aPawn = BLACK_A_PAWN;
  }
  
  if(this->pieces[king].isOnBoard)
    return true;
    
  // check if pawns are promoted to kings
  for(unsigned char i = 0; i < 8; i++)
  {
    Piece p = this->pieces[aPawn+i];
    if(p.isOnBoard && p.type == KING)
      return true;
  }
  
  return false;
}

GameResult Board::getGameResult(bool checkMoves) const
{
  bool whiteKing = hasKing(WHITE);
  bool blackKing = hasKing(BLACK);
  
  if(!whiteKing && !blackKing)
    return STALEMATE;
  else if(!whiteKing)
    return BLACK_VICTORY;
  else if (!blackKing)
    return WHITE_VICTORY;
  if(halfMoveClock >= 150)
    return STALEMATE;
  
  if(checkMoves)
    if(getValidMoves(turn).size() == 0)
      return STALEMATE;
    
  return ONGOING;
}

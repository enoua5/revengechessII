#include "game/board.h"
#include "game/game.h"
#include "ui/ui.h"
#include "pieceID.h"
#include "game/square.h"
#include "version.h"
#include "engine/engine.h"
#include <iostream>
#include <string>

Version UI::version = Version("Revenge Chess Viewer", 'c', 1, 1, 0);

void UI::printBoard(const Board& b)
{
  std::cout << std::endl;
  std::cout << "+-ABCDEFGH-+" << std::endl;
  std::cout << "|          |" << std::endl;
  for(int r = 7; r >= 0; r--)
  {
    std::cout << r+1 << " ";
    for(int f = 0; f < 8; f++)
    {
      int i = r*8 + f;
      char t = pieceToken(b, b.playField[i]);
      std::cout << t;
    }
    std::cout << " " << r+1 << std::endl;
  }
  std::cout << "|          |" << std::endl;
  std::cout << "+-ABCDEFGH-+" << std::endl;
}

void UI::startUI(Game& game)
{
  Engine engine;
  
  while(game.board.getGameResult() == ONGOING)
  {
    UI::printBoard(game.board);
    
    std::string input;
    std::cin >> input;
    //input = "play";
    
    if(input == "moves")
    {
      std::vector<Move> moves = game.board.getValidMoves();
      std::cout << moves.size() << std::endl;
      
      for(std::vector<Move>::iterator i = moves.begin(); i != moves.end(); i++)
        std::cout << (*i).toString() << std::endl;
    }
    else if(input == "back")
    {
      game.takeBack();
    }
    else if(input == "solve")
    {
      SearchResult ms = engine.solve(game.board);
      
      std::cout << ms.pv.back().toString() << ", score: " << ms.score << std::endl;
      std::cout << ms.depth << " ply, res: " << ms.result << std::endl;
    }
    else if(input == "play")
    {
      SearchResult ms = engine.solve(game.board);
      
      std::cout << ms.pv.back().toString() << ", score: " << ms.score << std::endl;
      std::cout << ms.depth << " ply, res: " << ms.result << std::endl;
      for(std::vector<Move>::iterator i = ms.pv.end(); i != ms.pv.begin(); /**/)
        std::cout << (*(--i)).toString() << " ";
      
      game.commitMove(ms.pv.back());
    }
    else if(input == "pass")
    {
      std::cout << (char)(game.board.pawnEnPassantFile+'A') << std::endl;
    }
    else if(input == "version")
    {
      Game::version.print();
      UI::version.print();
      Engine::version.print();
    }
    else if(input == "exit")
    {
      break;
    }
    else
    {
      try
      {
        Move m = UI::parseMove(input);
        game.commitMove(m);
      }
      catch(const invalid_move& e)
      {
        std::cout << "That is not a valid move" << std::endl;
      }
      
    }
  }
  UI::printBoard(game.board);
}

Move UI::parseMove(std::string s)
{
  if(s.size() < 4)
    return Move(Square(-1,-1), Square(-1,-1));
  
  unsigned char file_from = s[0];
  unsigned char rank_from = s[1];
  unsigned char file_to = s[2];
  unsigned char rank_to = s[3];
  
  // to lowecase
  file_from |= 0x20;
  file_to |= 0x20;
  
  // to 0-7 range
  file_from -= 'a';
  file_to -= 'a';
  
  rank_from -= '1';
  rank_to -= '1';
  
  PieceType p = PAWN;
  if(s.size() >= 5)
  {
    switch(s[4] | 0x20)
    {
      case 'r': p = ROOK;
      break;
      case 'n': p = KNIGHT;
      break;
      case 'b': p = BISHOP;
      break;
      case 'k': p = KING;
      break;
      case 'q': p = QUEEN;
      break;
    }
  }
  return Move(Square(file_from, rank_from), Square(file_to, rank_to), p);  
}

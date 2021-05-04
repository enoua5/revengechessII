#include "game/game.h"
#include "ui/ui.h"

int main()
{
  Game g;
  UI::startUI(g);
  /*
  std::cout << std::endl;
  g.board = g.board.makeMove(Move(Square(1,0), Square(2,2)));
  printBoard(g.board);
  std::cout << std::endl;
  g.board = g.board.makeMove(Move(Square(0,6), Square(0,5)));
  printBoard(g.board);
  std::cout << std::endl;
  g.board = g.board.makeMove(Move(Square(1,1), Square(1,2)));
  printBoard(g.board);
  std::cout << std::endl;
  g.board = g.board.makeMove(Move(Square(0,5), Square(0,4)));
  printBoard(g.board);
  std::cout << std::endl;
  g.board = g.board.makeMove(Move(Square(2,0), Square(0,2)));
  printBoard(g.board);
  std::cout << std::endl;
  g.board = g.board.makeMove(Move(Square(1,6), Square(1,4)));
  printBoard(g.board);
  std::cout << std::endl;
  g.board = g.board.makeMove(Move(Square(3,1), Square(3,2)));
  printBoard(g.board);
  std::cout << std::endl;
  g.board = g.board.makeMove(Move(Square(0,7), Square(0,6)));
  printBoard(g.board);
  std::cout << std::endl;
  g.board = g.board.makeMove(Move(Square(3,0), Square(3,1)));
  printBoard(g.board);
  std::cout << std::endl;
  g.board = g.board.makeMove(Move(Square(0,6), Square(0,7)));
  printBoard(g.board);
  std::cout << std::endl;
  g.board = g.board.makeMove(Move(Square(4,0), Square(2,0)));
  printBoard(g.board);
  
  std::list<Move> moves = g.board.getValidMoves();
  std::cout << moves.size() << std::endl;
  
  for(std::list<Move>::iterator i = moves.begin(); i != moves.end(); i++)
    std::cout << (*i).toString() << std::endl;
  */
  
  return 0;
}

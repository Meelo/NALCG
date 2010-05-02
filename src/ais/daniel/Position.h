#ifndef _NALCG_POSITION_H_
#define _NALCG_POSITION_H_

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <limits>
#include "MovementGenerator.h"

class Position
{
 private:
  char board[8][8];
  bool whiteToMove;
  bool isCheck;
  bool gameOver;
  bool quiet;
  MovementGenerator *generator;
  std::vector<std::string> legalMoves;
  bool testLeafNode();
  char translate(char piece);
  int pawnIsolationPenalty(int j, bool white);
  int pawnBackwardPenalty(int i, int j, bool white);
  int pawnAdvancementBonus(int i, int j, bool white);
  int pawnProximityToKingBonus(int i, int j, bool white);
  int centreBonus(int x);
  int bishopControlBonus(int i, int j, bool white);
  int rookControlBonus(int i, int j, bool white);
  int xrayControlBonus(int i, int j, bool white);

 public:
  Position(MovementGenerator *mg);
  Position(Position *p, int mv);
  Position(MovementGenerator *mg, char b[8][8], bool white);
  void reset();
  void move(std::string mv);
  void print();
  void getBoard(char b[8][8], int *wkx, int *wky, int *bkx, int *bky);
  bool isWhiteToMove();
  bool isGameOver();
  bool isPositionQuiet();
  int evaluate();
  std::vector<std::string>* getLegalMoves();
  MovementGenerator* getMovementGenerator();
  long getHash();
};

#endif // _NALCG_POSITION_H_

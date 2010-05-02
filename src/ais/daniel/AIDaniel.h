#ifndef _NALCG_AI_DANIEL_H_
#define _NALCG_AI_DANIEL_H_

#include "Position.h"

#include <string>
#include <algorithm>
#include <map>

#define CUTOFFDEPTH 2

class AIDaniel {
 private:
  Position *position;
  std::string bestMove;
  int alphaBeta(Position *pos, int depth, int alpha, int beta, bool max);

 public:
  AIDaniel(Position *p);
  void makeMove(std::string move);
  std::string getNextMove();
  Position* getPosition();
  void setPosition(Position *p);
  ~AIDaniel();
};

#endif // _NALCG_AI_DANIEL_H_

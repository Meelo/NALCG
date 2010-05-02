#include <string>
#include <algorithm>
#include <map>
#include "Position.h"

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

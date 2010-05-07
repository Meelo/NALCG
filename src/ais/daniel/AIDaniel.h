#include <string>
#include <algorithm>
#include <map>
#include "Position.h"

#define CUTOFFDEPTH 4

class AIDaniel {
 private:
	Position *position;
	int bestMove;
	//std::map<long,int> visited;
	int alphaBeta(Position *pos, int depth, int alpha, int beta, bool max);

 public:
	AIDaniel(Position *p);
	int getNextMove();
	Position* getPosition();
	void setPosition(Position *p);
	~AIDaniel();
};

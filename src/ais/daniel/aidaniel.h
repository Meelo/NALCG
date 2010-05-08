#ifndef _NALCG_AIDANIEL_H_
#define _NALCG_AIDANIEL_H_

#include "position.h"

#include <string>
#include <algorithm>
#include <map>

#define CUTOFFDEPTH 3 // Actually one higher.

class AIDaniel {
 private:
    Position *position;
    int bestMove;
    int alphaBeta(Position *pos, int depth, int alpha, int beta, bool max);

 public:
    AIDaniel(Position *p);
    int getNextMove();
    Position* getPosition();
    void setPosition(Position *p);
    ~AIDaniel();
};
#endif // _NALCG_AIDaniel_H_

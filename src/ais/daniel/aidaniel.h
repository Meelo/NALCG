#ifndef _NALCG_AIDANIEL_H_
#define _NALCG_AIDANIEL_H_

#include "position.h"

#include <string>
#include <algorithm>
#include <map>

class AIDaniel {
 private:
    static const int QUIESCENCE_SEARCH_MAX_ADDITIONAL_DEPTH = 1;

    Position *position;
    int bestMove;
    bool isInterrupted;
    int cutoffDepth;
    int alphaBeta(Position *pos, int depth, int alpha, int beta, bool max);

 public:
    static const int DEFAULT_CUTOFF_DEPTH = 4;

    AIDaniel(Position *p);
    int getNextMove();
    void setPosition(Position *p);
    void interrupt();
    void setCutoffDepth(int cutoffDepth);
    ~AIDaniel();
};
#endif // _NALCG_AIDaniel_H_

#include "aidaniel.h"

AIDaniel::AIDaniel(Position *p) : position(p), isInterrupted(false), cutoffDepth(DEFAULT_CUTOFF_DEPTH) {
}

inline int AIDaniel::alphaBeta(Position *pos, int depth, int alpha, int beta, bool max) {
    if (isInterrupted)
    {
        return 0;
    }
    if ((depth >= cutoffDepth && pos->getNoisiness() < 50)
        || depth >= cutoffDepth + QUIESCENCE_SEARCH_MAX_ADDITIONAL_DEPTH
        || pos->isGameOver()) {
            return pos->evaluate();
    }
    if (max) {
        for (unsigned int i = 0; i < pos->getLegalMoves()->size(); ++i) {
            Position child(pos, i);
            beta = std::min(beta, alphaBeta(&child, depth + 1, alpha, beta, !max));
            if (beta <= alpha) {
                return alpha;
            }
        }
        return beta;
    }
    else {
        for (unsigned int i = 0; i < pos->getLegalMoves()->size(); ++i) {
            Position child(pos, i);
            alpha = std::max(alpha, alphaBeta(&child, depth + 1, alpha, beta, !max));
            if (beta <= alpha) {
                return beta;
            }
        }
        return alpha;
    }
}

int AIDaniel::getNextMove() {
    isInterrupted = false;
    std::vector<int> values;
    int best = 0;
    if (position->isWhiteToMove()) {
        for (unsigned int i = 0; i < position->getLegalMoves()->size(); ++i) {
            Position child(position, i);
            values.push_back(alphaBeta(&child, 1, std::numeric_limits<int>::min(),std::numeric_limits<int>::max(),true));
            //std::cout << "white: " << i << " " << values.at(i) << " " << position->getLegalMoves()->at(i) << std::endl;
        }
        int max = std::numeric_limits<int>::min();
        for (unsigned int i = 0; i < values.size(); ++i) {
            if (max < values.at(i)) {
                max = values.at(i);
                best = i;
            }
        }
    }
    else {
        for (unsigned int i = 0; i < position->getLegalMoves()->size(); ++i) {
            Position child(position, i);
            values.push_back(alphaBeta(&child, 1, std::numeric_limits<int>::min(),std::numeric_limits<int>::max(),false));
            //std::cout << "black: " << i << " " << values.at(i) << " " << position->getLegalMoves()->at(i) << std::endl;
        }
        int min = std::numeric_limits<int>::max();
        for (unsigned int i = 0; i < values.size(); ++i) {
            if (min > values.at(i)) {
                min = values.at(i);
                best = i;
            }
        }
    }
    bestMove = position->getLegalMoves()->at(best);
    position->move(bestMove, 0);
    return bestMove;
}

void AIDaniel::setPosition(Position *p) {
    delete position;
    position = p;
}

void AIDaniel::interrupt()
{
    isInterrupted = true;
}

void AIDaniel::setCutoffDepth(int cutoffDepth)
{
    this->cutoffDepth = cutoffDepth;
}

AIDaniel::~AIDaniel() {
    delete position;
}

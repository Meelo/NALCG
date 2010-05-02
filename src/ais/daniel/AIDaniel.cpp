#include "AIDaniel.h"

AIDaniel::AIDaniel(Position *p) : position(p) {
}

int AIDaniel::alphaBeta(Position *pos, int depth, int alpha, int beta, bool max) {
    if (depth >= CUTOFFDEPTH || pos->isGameOver()) {
        return pos->evaluate();
    }
    if (max) {
        for (std::size_t i = 0; i < pos->getLegalMoves()->size(); ++i) {
            Position child(pos, i);
            beta = std::min(beta, alphaBeta(&child, depth + 1, alpha, beta, !max));
            if (beta <= alpha) {
                return alpha;
            }
        }
        return beta;
    }
    else {
        for (std::size_t i = 0; i < pos->getLegalMoves()->size(); ++i) {
            Position child(pos, i);
            alpha = std::max(alpha, alphaBeta(&child, depth + 1, alpha, beta, !max));
            if (beta <= alpha) {
                return beta;
            }
        }
        return alpha;
    }
}

void AIDaniel::makeMove(std::string move) {
    position->move(move);
}

std::string AIDaniel::getNextMove() {
    std::vector<int> values;
    int best = 0;
    if (position->isWhiteToMove()) {
        for (std::size_t i = 0; i < position->getLegalMoves()->size(); ++i) {
            Position child(position, i);
            values.push_back(alphaBeta(&child, 0, std::numeric_limits<int>::min(),std::numeric_limits<int>::max(),true));
        }
        int max = std::numeric_limits<int>::min();
        for (std::size_t i = 0; i < values.size(); ++i) {
            if (max < values.at(i)) {
                max = values.at(i);
                best = i;
            }
        }
    }
    else {
        for (std::size_t i = 0; i < position->getLegalMoves()->size(); ++i) {
            Position child(position, i);
            values.push_back(alphaBeta(&child, 0, std::numeric_limits<int>::min(),std::numeric_limits<int>::max(),false));
            std::cout << i << " " << values.at(i) << " " << position->getLegalMoves()->at(i) << std::endl;
        }
        int min = std::numeric_limits<int>::max();
        for (std::size_t i = 0; i < values.size(); ++i) {
            if (min > values.at(i)) {
                min = values.at(i);
                best = i;
            }
        }
    }
    bestMove = position->getLegalMoves()->at(best);
    position->move(bestMove);
    return bestMove;
}


Position* AIDaniel::getPosition() {
    return position;
}

void AIDaniel::setPosition(Position *p) {
    delete position;
    position = p;
}

AIDaniel::~AIDaniel() {
    delete position;
}

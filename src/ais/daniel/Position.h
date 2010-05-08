#ifndef _NALCG_Position_H_
#define _NALCG_Position_H_

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <limits>
#include "MovementGenerator.h"

#define UNPACK_I1(x) (((x) >> 12) & 7)
#define UNPACK_J1(x) (((x) >> 8) & 7)
#define UNPACK_I2(x) (((x) >> 4) & 7)
#define UNPACK_J2(x) ((x) & 7)

class Position
{
private:
    char board[8][8];
    bool whiteToMove;
    bool isCheck;
    bool gameOver;
    MovementGenerator *generator;
    std::vector<int> legalMoves;
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
    void move(int mv, int promotion);
    void print();
    void getBoard(char b[8][8], int *wkx, int *wky, int *bkx, int *bky);
    bool isWhiteToMove();
    bool isGameOver();
    int evaluate();
    std::vector<int>* getLegalMoves();
    MovementGenerator* getMovementGenerator();
    long getHash();
};
#endif // _NALCG_Position_H_

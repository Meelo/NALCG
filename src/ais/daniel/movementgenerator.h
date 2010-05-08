#ifndef _NALCG_MOVEMENT_GENERATOR_H_
#define _NALCG_MOVEMENT_GENERATOR_H_

#include <string>
#include <vector>
#include <iostream>

class Position;

class MovementGenerator
{
 private:
    char board[8][8];
    bool whiteToMove;
    int wkx, wky, bkx, bky;
    std::vector<int> *legalMoves;
    void addLegalRookMoves(int i, int j);
    void addLegalBishopMoves(int i, int j);
    void addLegalKnightMoves(int i, int j);
    void addLegalKingMoves(int i, int j);
    void addLegalPawnMoves(int i, int j);
    void addLegalCastlingMoves();
    bool isPieceHostile(int i, int j);
    void restore(int i1, int j1, int i2, int j2, char memory);
    std::string convertToStringCoordinates(int i1, int j1, int i2, int j2);

 public:
    MovementGenerator();
    void setPosition(Position *p);
    void getAllLegalMoves(std::vector<int> *lm);
    bool isKingSafe(int i1, int j1, int i2, int j2);
};
#endif // _NALCG_MovementGenerator_H_

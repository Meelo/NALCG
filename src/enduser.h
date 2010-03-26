#ifndef _NALCG_ENDUSER_H_
#define _NALCG_ENDUSER_H_

#include "logic/board.h"

class EndUser
{
public:
    EndUser() { };
    virtual ~EndUser() { }
    
    virtual void move(int fromX, int fromY, int toX, int toY) = 0;
    virtual void setBoard(const Board& board, unsigned int round) = 0;
};

#endif // _NALCG_ENDUSER_H_

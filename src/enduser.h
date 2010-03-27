#ifndef _NALCG_ENDUSER_H_
#define _NALCG_ENDUSER_H_

#include "logic/board.h"

class Middleman;
class EndUser
{
public:
    EndUser() : mMiddleman(0) { };
    virtual ~EndUser() { }

    virtual void move(int fromX, int fromY, int toX, int toY) = 0;
    virtual void setBoard(const Board* const board, unsigned int round) = 0;
    virtual void setControl(bool white, bool black) = 0;
    virtual void setMiddleman(Middleman* middleman) { mMiddleman = middleman; }
    virtual Middleman* getMiddleman() const { return mMiddleman; }
protected:
    Middleman* mMiddleman;
};

#endif // _NALCG_ENDUSER_H_

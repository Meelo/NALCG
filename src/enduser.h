#ifndef _NALCG_ENDUSER_H_
#define _NALCG_ENDUSER_H_

#include "logic/board.h"

class Middleman;
class EndUser
{
public:
    EndUser() : mMiddleman(0) { };
    virtual ~EndUser() { }

    // Note: Do not use board as a member variable as it can change and be deleted.
    // It's not safe to use it after these function calls have returned.
    virtual void init(const Board* board, Middleman* middleman) { mMiddleman = middleman; }
    virtual void setBoard(const Board* board, unsigned int round) = 0;

    virtual void move(int fromX, int fromY, int toX, int toY,
        bool continuous = false) = 0;
    virtual void promoteMove(int fromX, int fromY, int toX, int toY,
        unsigned int promoteTo) = 0;
    virtual void setControl(bool white, bool black) = 0;
    virtual Middleman* getMiddleman() const { return mMiddleman; }
protected:
    Middleman* mMiddleman;
};

#endif // _NALCG_ENDUSER_H_

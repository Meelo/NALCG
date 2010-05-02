#ifndef _NALCG_A_I_H_
#define _NALCG_A_I_H_

// class includes

// system includes

class Board;
class Middleman;

class AI
{
public:
    AI() : mMiddleman(0) { }
    virtual ~AI() { }

    // Note: Do not use board as a member variable as it can change and be deleted.
    // It's not safe to use it after these function calls have returned.
    virtual void init(const Board* board, Middleman* middleman) { mMiddleman = middleman; }
    virtual void setBoard(const Board* board, unsigned int round) = 0;

    virtual void setControl(bool white, bool black) = 0;

protected:
    Middleman* mMiddleman;
};

#endif // _NALCG_A_I_H_

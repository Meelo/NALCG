#ifndef _NALCG_A_I_ADAPTER_H_
#define _NALCG_A_I_ADAPTER_H_

// system includes

// class includes
#include "../ai.h"
#include "movementgenerator.h"
#include "aidaniel.h"
#include "../../middleman.h"
#include "../../logic/chessboard.h"
#include "../../logic/board.h"

#include <boost/thread/thread.hpp>

#include <string>

class AIAdapter : public AI
{
public:
    AIAdapter();
    virtual ~AIAdapter();

    virtual void init(const Board* board, Middleman* middleman);

    virtual void setBoard(const Board* board, unsigned int round);

    virtual void setControl(bool white, bool black);


protected:
    AIDaniel *ai;
    MovementGenerator *mg;
    bool controlWhite;
    bool controlBlack;
    boost::mutex mutex;
    boost::thread* thread;
    bool moveAllowed;

    virtual void parsePosition(const Board* board, bool whiteToMove);
    virtual void makeMoveIfInControl();
    void stopThread();
    void startThreadIfInControl();
};

#endif // _NALCG_A_I_ADAPTER_H_

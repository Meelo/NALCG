// system includes

// class dependencies
#include "aiadapter.h"

AIAdapter::AIAdapter()
    : ai(0)
    , controlWhite(false)
    , controlBlack(false)
    , thread(0)
    , moveAllowed(false)
    , whiteCutoffDepth(AIDaniel::DEFAULT_CUTOFF_DEPTH)
    , blackCutoffDepth(AIDaniel::DEFAULT_CUTOFF_DEPTH)
{
    mg = new MovementGenerator();
}

AIAdapter::~AIAdapter()
{
    moveAllowed = false;
    if (thread)
    {
        ai->interrupt();
        thread->join();
        delete thread;
    }
    delete ai;
    delete mg;
}

void AIAdapter::init(const Board* board, Middleman* middleman)
{
    AI::init(board, middleman);
    Position *p = new Position(mg);
    ai = new AIDaniel(p);
    parsePosition(board, true);
}

void AIAdapter::setBoard(const Board* board, unsigned int round)
{
    parsePosition(board, round % 2 == 0);
}

void AIAdapter::parsePosition(const Board* board, bool whiteToMove)
{
    stopThread();

    char charBoard[8][8];
    board->createCharBoard(&charBoard[0][0], 8, 8);
    Position *pos = new Position(mg, charBoard, whiteToMove);
    ai->setPosition(pos);

    startThreadIfInControl();
}

void AIAdapter::setControl(bool white, bool black)
{
    controlWhite = white;
    controlBlack = black;
    startThreadIfInControl();
}

void AIAdapter::setCutoffDepth(int white, int black)
{
    whiteCutoffDepth = white;
    blackCutoffDepth = black;
}


void AIAdapter::stopThread()
{
    if (thread)
    {
        moveAllowed = false;
        if (thread->get_id() != boost::this_thread::get_id())
        {
            ai->interrupt();
            thread->join();
            delete thread;
            thread = 0;
        }
    }
}

void AIAdapter::startThreadIfInControl()
{
    if ((mMiddleman->getGameConditionMask() & (ChessBoard::CHECKMATE | ChessBoard::DRAW)) == 0)
    {
        bool whiteToMove = mMiddleman->getGameLog().size() % 2 == 0;

        if ((whiteToMove && controlWhite) || (!whiteToMove && controlBlack))
        {
            stopThread();
            moveAllowed = true;
            ai->setCutoffDepth(whiteToMove ? whiteCutoffDepth : blackCutoffDepth);
            thread = new boost::thread(boost::bind(&AIAdapter::makeMoveIfInControl, this));
        }
    }
}

void AIAdapter::makeMoveIfInControl()
{
    int mv;
    {
        boost::mutex::scoped_lock l(mutex);
        mv = ai->getNextMove();
    }

    if (moveAllowed)
    {
        std::cout << (mMiddleman->getGameLog().size() % 2 == 0 ? "White" : "Black") << " moves: " << UNPACK_J1(mv) << " " << UNPACK_I1(mv) << " " <<  UNPACK_J2(mv) << " " <<  UNPACK_I2(mv) << std::endl;
        mMiddleman->move(UNPACK_J1(mv), UNPACK_I1(mv), UNPACK_J2(mv), UNPACK_I2(mv), ChessBoard::PROMOTE_TO_QUEEN);
    }
}

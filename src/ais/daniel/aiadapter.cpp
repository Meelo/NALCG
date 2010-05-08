// system includes

// class dependencies
#include "aiadapter.h"

AIAdapter::AIAdapter() : ai(0), controlWhite(false), controlBlack(false)
{
    mg = new MovementGenerator();
}

AIAdapter::~AIAdapter()
{
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
    char charBoard[8][8];
    board->createCharBoard(&charBoard[0][0], 8, 8);
    Position *pos = new Position(mg, charBoard, whiteToMove); // luodaan jotenkin ton charboardin perusteella tms
    ai->setPosition(pos);

    boost::thread thread(boost::bind(&AIAdapter::makeMoveIfInControl, this));
}

void AIAdapter::setControl(bool white, bool black)
{
    controlWhite = white;
    controlBlack = black;
    boost::thread thread(boost::bind(&AIAdapter::makeMoveIfInControl, this));
}

void AIAdapter::makeMoveIfInControl()
{
    {
        boost::mutex::scoped_lock l(mutex);

        bool whiteToMove = mMiddleman->getGameLog().size() % 2 == 0;

        if ((whiteToMove && controlWhite) || (!whiteToMove && controlBlack))
        {
            int mv = ai->getNextMove();

            std::cout << UNPACK_J1(mv) << " " << UNPACK_I1(mv) << " " <<  UNPACK_J2(mv) << " " <<  UNPACK_I2(mv) << std::endl;
            mMiddleman->move(UNPACK_J1(mv), UNPACK_I1(mv), UNPACK_J2(mv), UNPACK_I2(mv), ChessBoard::PROMOTE_TO_QUEEN);
        }
    }
}

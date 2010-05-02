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
    bool whiteToMove = mMiddleman->getGameLog().size() % 2 == 0;

    if ((whiteToMove && controlWhite) || (!whiteToMove && controlBlack))
    {
        std::string move = ai->getNextMove();
        std::size_t x0, y0, x1, y1;

        // Tassa valissa parsetaan movesta noi ylemmalla rivilla olevat
        if (move.at(0) == 'O') {
            if (whiteToMove) {
                x0 = 7;
                y0 = 4;
                x1 = 7;
            }
            else {
                x0 = 0;
                y0 = 4;
                x1 = 0;
            }
            if (move.size() > 3) {
                y1 = 2;
            }
            else {
                y1 = 6;
            } 
        }
        else {
            y0 = move.at(0) - 'A';
            x0 = 8 - (move.at(1) - '0');
            y1 = move.at(3) - 'A';
            x1 = 8 - (move.at(4) - '0');
        }

        mMiddleman->move(y0, x0, y1, x1, ChessBoard::PROMOTE_TO_QUEEN);
    }
}

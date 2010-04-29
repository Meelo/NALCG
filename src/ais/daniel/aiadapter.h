#ifndef _NALCG_A_I_ADAPTER_H_
#define _NALCG_A_I_ADAPTER_H_

// system includes

// class includes
#include "../ai.h"
#include "MovementGenerator.h"
#include "../../middleman.h"
#include "AIDaniel.h"
#include "../../logic/chessboard.h"

class AIAdapter : public AI
{
public:
    AIAdapter() : ai(0), controlWhite(false), controlBlack(false)
    {
        mg = new MovementGenerator();
    }
    virtual ~AIAdapter()
    {
        delete ai;
        delete mg;
    }

    virtual void init(const Board* board, Middleman* middleman)
    {
        AI::init(board, middleman);
        Position *p = new Position(mg);
        ai = new AIDaniel(p);
        parsePosition(board, true);
    }

    virtual void setBoard(const Board* board, unsigned int round)
    {
        parsePosition(board, round % 2 == 0);
    }

    virtual void parsePosition(const Board* board, bool whiteToMove)
    {
        char charBoard[8][8];
        //board->getCharBoard(charBoard);
        Position *pos = new Position(mg, charBoard, whiteToMove); // luodaan jotenkin ton charboardin perusteella tms
        ai->setPosition(pos);

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

    virtual void setControl(bool white, bool black)
    {
        controlWhite = white;
        controlBlack = black;        
    }

protected:
    AIDaniel *ai;
    MovementGenerator *mg;
    bool controlWhite;
    bool controlBlack;
};

#endif // _NALCG_A_I_ADAPTER_H_

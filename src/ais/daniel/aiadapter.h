/*

#ifndef _NALCG_A_I_A_DAPTER_H_
#define _NALCG_A_I_A_DAPTER_H_

// system includes

// class includes

class AIADapter : public AI
{
public:
    AIADapter() : ai(0), controlWhite(false), controlBlack(false)
    {
    }

    virtual ~AIADapter()
    {
        delete ai;
    }

    virtual void init(const Board* board, Middleman* middleman)
    {
        AI::init(board, middleman);

        ai = new AIDaniel();
        parsePosition(board, true);
    }

    virtual void setBoard(const Board* board, unsigned int round)
    {
        parsePosition(board, round % 2 == 0);
    }

    virtual parsePosition(const Board* board, bool whiteToMove)
    {
        char charBoard[8][8]; // tai jotain siihen tyyliin
        board->getCharBoard(charBoard);
        
        Position pos; // luodaan jotenkin ton charboardin perusteella tms
        ai->setPosition(pos, whiteToMove);

        if ((whiteToMove && controlWhite) || (!whiteToMove && controlBlack))
        {
            std::string move = ai->getNextMove();

            std::size_t x0, y0, x1, y1;
            // Tassa valissa parsetaan movesta noi ylemmalla rivilla olevat

            mMiddleman->move(x0, y0, x1, y1, ChessBoard::PROMOTE_TO_QUEEN);
        }
    }

    virtual void setControl(bool white, bool black)
    {
        controlWhite = white;
        controlBlack = black;
    }



protected:
    AIDaniel* ai;
    bool controlWhite;
    bool controlBlack;
};

#endif // _NALCG_A_I_A_DAPTER_H_ */

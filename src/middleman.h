#ifndef _NALCG_MIDDLE_MAN_H_
#define _NALCG_MIDDLE_MAN_H_

// system includes

// class includes
#include "logic/board.h"
#include "logic/chesspieces-meta.h"

class MiddleMan
{
    // Constants

    // Members
    Board* board;
    Piece::Colour currentTurn;
    
    // Methods
    void playRound();
    void update();

public:
    MiddleMan();
    virtual ~MiddleMan() { }

    // Methods
    void startGame();
    Piece::Colour endGame();
    std::vector<std::size_t> getValidMovesAt(std::size_t x, std::size_t y) const;
    bool move(  std::size_t fromX, std::size_t fromY, 
                std::size_t toX,   std::size_t toY);
    void undo();

    // Getters
    
    // Setters
};

#endif // _NALCG_MIDDLE_MAN_H_

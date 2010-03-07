#ifndef _NALCG_CHESS_BOARD_H_
#define _NALCG_CHESS_BOARD_H_

// system includes
#include <iostream>

// class includes
#include "board.h"

class ChessBoard : public Board
{
    // Constants

    // Members

    // Methods

public:
    ChessBoard(std::size_t width = 8, std::size_t height = 8);
    virtual ~ChessBoard() { }

    // Methods

    // Getters
    
    // Setters
};

#endif // _NALCG_CHESS_BOARD_H_

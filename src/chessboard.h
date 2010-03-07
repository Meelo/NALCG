#ifndef _NALCG_CHESS_BOARD_H_
#define _NALCG_CHESS_BOARD_H_

// system includes
#include <iostream>
#include <vector>

// class includes
#include "board.h"
#include "square.h"

class ChessBoard : public Board
{
    // Constants
    static const std::size_t WIDTH = 8;
    static const std::size_t HEIGHT = 8;
    static const std::size_t BOARD_SIZE = 64;

    // Members

    // Methods

public:
    ChessBoard();
    virtual ~ChessBoard() { }

    // Methods
    static std::vector<Square> createBoard();

    // Getters
    
    // Setters
};

#endif // _NALCG_CHESS_BOARD_H_

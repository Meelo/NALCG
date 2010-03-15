#ifndef _NALCG_CHESS_BOARD_H_
#define _NALCG_CHESS_BOARD_H_

// system includes
#include <iostream>
#include <vector>

// class includes
#include "board.h"
#include "square.h"
#include "chesspieces-meta.h"

class ChessBoard : public Board
{
    // Constants
    static const std::size_t WIDTH = 8;
    static const std::size_t HEIGHT = 8;
    static const std::size_t BOARD_SIZE = 64;

    static const std::size_t BLACK_ROOK_LEFT = 0;
    static const std::size_t BLACK_KNIGHT_LEFT = 1;
    static const std::size_t BLACK_BISHOP_LEFT = 2;
    static const std::size_t BLACK_QUEEN = 3;
    static const std::size_t BLACK_KING = 4;
    static const std::size_t BLACK_BISHOP_RIGHT = 5;
    static const std::size_t BLACK_KNIGHT_RIGHT = 6;
    static const std::size_t BLACK_ROOK_RIGHT = 7;

    static const std::size_t WHITE_ROOK_LEFT = 56;
    static const std::size_t WHITE_KNIGHT_LEFT = 57;
    static const std::size_t WHITE_BISHOP_LEFT = 58;
    static const std::size_t WHITE_QUEEN = 59;
    static const std::size_t WHITE_KING = 60;
    static const std::size_t WHITE_BISHOP_RIGHT = 61;
    static const std::size_t WHITE_KNIGHT_RIGHT = 62;
    static const std::size_t WHITE_ROOK_RIGHT = 63;

    // Members

    // Methods

public:
    ChessBoard(const std::vector<Square>& squares);
    virtual ~ChessBoard() { };

    // Methods
    static std::vector<Square> createBoard();

    // Getters
    
    // Setters
};

#endif // _NALCG_CHESS_BOARD_H_

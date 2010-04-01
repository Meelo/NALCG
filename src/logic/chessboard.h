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
public:
    // Constants
    static const std::size_t WIDTH;
    static const std::size_t HEIGHT;
    static const std::size_t BOARD_SIZE;
    static const std::size_t BLACK_PAWN_ROW;
    static const std::size_t WHITE_PAWN_ROW;

    // default positions - black
    static const std::size_t BLACK_ROOK_LEFT;
    static const std::size_t BLACK_KNIGHT_LEFT;
    static const std::size_t BLACK_BISHOP_LEFT;
    static const std::size_t BLACK_QUEEN;
    static const std::size_t BLACK_KING;
    static const std::size_t BLACK_BISHOP_RIGHT;
    static const std::size_t BLACK_KNIGHT_RIGHT;
    static const std::size_t BLACK_ROOK_RIGHT;
    // default positions - white
    static const std::size_t WHITE_ROOK_LEFT;
    static const std::size_t WHITE_KNIGHT_LEFT;
    static const std::size_t WHITE_BISHOP_LEFT;
    static const std::size_t WHITE_QUEEN;
    static const std::size_t WHITE_KING;
    static const std::size_t WHITE_BISHOP_RIGHT;
    static const std::size_t WHITE_KNIGHT_RIGHT;
    static const std::size_t WHITE_ROOK_RIGHT;
    // piece symbols - black
    static const char BLACK_PAWN_SYMBOL;
    static const char BLACK_ROOK_SYMBOL;
    static const char BLACK_KNIGHT_SYMBOL;
    static const char BLACK_BISHOP_SYMBOL;
    static const char BLACK_QUEEN_SYMBOL;
    static const char BLACK_KING_SYMBOL;
    // piece symbols - white
    static const char WHITE_PAWN_SYMBOL;
    static const char WHITE_ROOK_SYMBOL;
    static const char WHITE_KNIGHT_SYMBOL;
    static const char WHITE_BISHOP_SYMBOL;
    static const char WHITE_QUEEN_SYMBOL;
    static const char WHITE_KING_SYMBOL;

    ChessBoard(const std::vector<Square>& squares);
    virtual ~ChessBoard() { };

    // Methods
    static std::vector<Square> createBoard();

    // Getters
    static bool getCoordinates(std::size_t index, std::size_t& column, std::size_t& row);
    static std::size_t getPosition(std::size_t column, std::size_t row);

    // Setters

private:
    // Constants

    // Members

    // Methods
};

#endif // _NALCG_CHESS_BOARD_H_

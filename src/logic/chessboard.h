#ifndef _NALCG_CHESS_BOARD_H_
#define _NALCG_CHESS_BOARD_H_

// class includes
#include "board.h"
#include "colour.h"
#include "square.h"
#include "chesspieces-meta.h"

// system includes
#include <iostream>
#include <vector>

class ChessBoard : public Board
{
public:
    // Constants
    static const std::size_t WIDTH;
    static const std::size_t HEIGHT;
    static const std::size_t BOARD_SIZE;
    static const std::size_t BLACK_PAWN_ROW;
    static const std::size_t WHITE_PAWN_ROW;
    static const std::size_t BLACK_PROMOTE_ROW;
    static const std::size_t WHITE_PROMOTE_ROW;

    static const unsigned int PROMOTE_TO_BISHOP;
    static const unsigned int PROMOTE_TO_ROOK;
    static const unsigned int PROMOTE_TO_KNIGHT;
    static const unsigned int PROMOTE_TO_QUEEN;


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

    static const char EMPTY_SYMBOL;

    ChessBoard(const std::vector<Square>& squares);
    virtual ChessBoard* clone() const { return new ChessBoard(*this); }
    virtual ~ChessBoard() { };

    // Methods
    static std::vector<Square> createBoard();
    virtual std::vector<std::size_t> getValidMoves(std::size_t location) const;

    static bool isUnderAttack(std::size_t location,
        const std::vector<Square>& squares,
        std::size_t moveFrom = BOARD_SIZE, std::size_t moveTo = BOARD_SIZE);

    // Getters
    static bool getCoordinates(std::size_t index, std::size_t& column, std::size_t& row);
    static std::size_t getPosition(std::size_t column, std::size_t row);

    // Setters

protected:
    // Constants
    static const int RECTANGLE_DIRECTIONS_X[];
    static const int RECTANGLE_DIRECTIONS_Y[];
    static const int DIAGONAL_DIRECTIONS_X[];
    static const int DIAGONAL_DIRECTIONS_Y[];
    static const int KNIGHT_DIRECTIONS_X[];
    static const int KNIGHT_DIRECTIONS_Y[];

    // Methods
    static bool isBishop(char symbol);
    static bool isRook(char symbol);
    static bool isKnight(char symbol);
    static bool isQueen(char symbol);
    static bool areOppositeColour(char symbol1, char symbol2);

    // Methods
    virtual bool isPromotable(std::size_t moveFrom, std::size_t moveTo) const;
    virtual void promote(std::size_t location, unsigned int promoteTo);
    std::size_t findKing(Colour colour) const;
};

#endif // _NALCG_CHESS_BOARD_H_

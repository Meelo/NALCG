#ifndef _NALCG_BOARD_H_
#define _NALCG_BOARD_H_

// system includes
#include <iostream>
#include <vector>

// class includes
#include "square.h"

class Board
{
public:
    // Constants
    static const unsigned int MOVE_OK           = 1 << 0;
    static const unsigned int INVALID_TURN      = 1 << 1;
    static const unsigned int INVALID_MOVE      = 1 << 2;
    static const unsigned int PROMOTION_REQUEST = 1 << 3;

    Board(const std::vector<Square>& squares, std::size_t width = 8, std::size_t height = 8);
    Board(const Board& orig);
    virtual ~Board();

    // Methods
    static std::vector<Square> createBoard(std::size_t width = 8, std::size_t height = 8);
    std::vector<std::size_t> getValidMoves(std::size_t location);
    std::vector<std::size_t> getValidMoves(std::size_t x, std::size_t y);
    static bool getCoordinates(std::size_t index, std::size_t& column,
        std::size_t& row, std::size_t boardWidth, std::size_t boardHeight);
    static std::size_t getPosition(std::size_t column, std::size_t row,
        std::size_t boardWidth, std::size_t boardHeight);
    unsigned int move(  std::size_t fromX,  std::size_t fromY,
                        std::size_t toX,    std::size_t toY,
                        Piece::Colour player, unsigned int promoteTo = 0);
    void printBoard() const;
    void initRoundSpecificState();

    // Getters
    std::size_t getWidth() const { return width; }
    std::size_t getHeight() const { return height; }
    std::size_t getTotalSize() const { return squares.size(); }
    char getSymbolAt(std::size_t column, std::size_t row) const;
    std::string getNameAt(std::size_t column, std::size_t row) const;

    // Setters

protected:
    // Members
    std::vector<Square> squares;
    std::vector<Piece*> deadPieces;

    std::size_t width;
    std::size_t height;

    unsigned int rounds;

    // Methods
    bool isMoveValid(std::size_t moveFrom, std::size_t moveTo,
        Piece::Colour player, unsigned int& mask) const;
    bool getCoordinates(std::size_t index, std::size_t& column,
        std::size_t& row) const;
    std::size_t getPosition(std::size_t column, std::size_t row) const;
    void move(std::size_t moveFrom, std::size_t moveTo);
    virtual bool isPromotable(std::size_t moveFrom, std::size_t moveTo) const = 0;
    virtual void promote(std::size_t location, unsigned int promoteTo) = 0;
};

#endif // _NALCG_BOARD_H_

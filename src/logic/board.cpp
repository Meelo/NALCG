// system includes

// class dependencies
#include "board.h"

Board::Board(const std::vector<Square>& squares, std::size_t width, 
    std::size_t height) : squares(squares), width(width), height(height)
{

}

Board::~Board()
{
    
}

std::vector<Square> Board::createBoard(std::size_t width, std::size_t height)
{
    std::vector<Square> squares;
    std::size_t boardSize = width * height;
    for (std::size_t i = 0; i < boardSize; ++i)
    {
        Square square;
        squares.push_back(square);
    }

    return squares;
}

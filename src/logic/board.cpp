// system includes

// class dependencies
#include "board.h"
#include "chesspieces-meta.h"

Board::Board(const std::vector<Square>& squares, std::size_t width, 
    std::size_t height) : squares(squares), width(width), height(height)
{

}

Board::~Board()
{
    for (std::size_t i = 0; i < squares.size(); ++i)
    {
        delete squares.at(i).removePiece();
    }
}


std::vector<std::size_t> Board::getValidMoves(std::size_t location)
{
    if (location >= squares.size()) { return std::vector<std::size_t>(); }
    Piece* piece = squares.at(location).getPiece();
    if (!piece) { return std::vector<std::size_t>(); }

    return piece->getValidMoves(location, squares);
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

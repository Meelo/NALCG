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

// public

std::vector<std::size_t> Board::getValidMoves(std::size_t location)
{
    if (location >= squares.size()) { return std::vector<std::size_t>(); }
    Piece* piece = squares.at(location).getPiece();
    if (!piece) { return std::vector<std::size_t>(); }

    return piece->getValidMoves(location, squares);
}

std::vector<std::size_t> Board::getValidMoves(std::size_t x, std::size_t y)
{
    return getValidMoves(getPosition(x, y));
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

bool Board::getCoordinates(std::size_t index, std::size_t& column,
    std::size_t& row, std::size_t boardWidth, std::size_t boardHeight)
{
    std::size_t boardSize = boardWidth * boardHeight;
    if (index >= boardSize)
    {
        column = -1;
        row = -1;
        return false;
    }

    column = index % boardWidth;
    row = index / boardWidth;

    return true;
}

std::size_t Board::getPosition(std::size_t column, std::size_t row,
    std::size_t boardWidth, std::size_t boardHeight)
{
    if (column >= boardWidth || row >= boardHeight)
    {
        return -1;
    }

    return (row * boardWidth + column);
}

bool Board::move(std::size_t fromX, std::size_t fromY,
    std::size_t toX, std::size_t toY)
{
    // first merge two dimensions into one.
    std::size_t moveFrom = getPosition(fromX, fromY);
    std::size_t moveTo = getPosition(toX, toY);
    if (isMoveValid(moveFrom, moveTo))
    {
        // Validation passed, movement shall be done.

        squares.at(moveTo).addPiece(squares.at(moveFrom).getPiece());
        squares.at(moveFrom).removePiece();

        return true;
    }

    return false;
}

// private


bool Board::isMoveValid(std::size_t moveFrom, std::size_t moveTo) const
{
    // TODO do the actual validation
    // Validation should be about going through current states (is checked?),
    // going through selected unit's valid moves, etc.

    return moveFrom != moveTo;
}

// just forward this to the static version of this method.
bool Board::getCoordinates(std::size_t index, std::size_t& column,
    std::size_t& row) const
{
    return getCoordinates(index, column, row, width, height);
}

// just forward this to the static version of this method.
std::size_t Board::getPosition(std::size_t column, std::size_t row) const
{
    return getPosition(column, row, width, height);
}


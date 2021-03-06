// class dependencies
#include "board.h"
#include "chesspieces-meta.h"

// system includes
#include <iostream>

Board::Board(const std::vector<Square>& squares, std::size_t width,
    std::size_t height) : squares(squares), width(width), height(height),
    rounds(0)
{

}


Board::Board(const Board& orig)
{
    squares = orig.squares;
    for (std::size_t i = 0; i < orig.deadPieces.size(); ++i)
    {
        deadPieces.push_back(orig.deadPieces.at(i)->clone());
    }
    width = orig.width;
    height = orig.height;
    rounds = orig.rounds;
}


Board::~Board()
{
    deleteAndClear(deadPieces);

    while (!squares.empty())
    {
        delete squares.back().removePiece();
        squares.pop_back();
    }
}


// public


std::vector<std::size_t> Board::getValidMoves(std::size_t x, std::size_t y) const
{
    return getValidMoves(getPosition(x, y));
}


bool Board::hasValidMoves(std::size_t x, std::size_t y) const
{
    return hasValidMoves(getPosition(x, y));
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


unsigned int Board::move(   std::size_t& fromX, std::size_t& fromY,
                            std::size_t& toX,   std::size_t& toY,
                            Colour player, unsigned int promoteTo)
{
    unsigned int retValue = 0;
    // first merge two dimensions into one.
    std::size_t moveFrom = getPosition(fromX, fromY);
    std::size_t moveTo = getPosition(toX, toY);
    if (isMoveValid(moveFrom, moveTo, player, retValue))
    {
        bool promotable = isPromotable(moveFrom, moveTo);
        if (!promoteTo && promotable) {
            retValue |= PROMOTION_REQUEST;
        }
        else
        {
            if (promotable)
            {
                promote(moveFrom, promoteTo);
                retValue |= PROMOTION_OK;
            }

            // Try if moved piece has a special move.
            // Special move will set fromX, fromY, toX and toY
            // if special move was made and then return's true
            squares.at(moveFrom).getPiece()->trySpecialMove(fromX, fromY, toX,
                toY, squares);

            if (squares.at(moveTo).hasPiece())
            {
                // Validation passed
                // First kill unit if such exists.
                deadPieces.push_back(squares.at(moveTo).removePiece());
            }

            // Then movement shall be done.
            move(moveFrom, moveTo);
            retValue |= MOVE_OK;
        }
    }

    return retValue;
}


char Board::getSymbolAt(std::size_t column, std::size_t row) const
{
    return squares.at(getPosition(column, row)).getSymbolOfPiece();
}


std::string Board::getNameAt(std::size_t column, std::size_t row) const
{
    return squares.at(getPosition(column, row)).getNameOfPiece();
}


void Board::printBoard() const
{
    for (std::size_t i = 0; i < squares.size(); ++i)
    {
        if (i % width == 0)
        {
            std::cout << std::endl;
        }
        char c = squares.at(i).getSymbolOfPiece(true);
        if (!c) c = '-';
        std::cout << c;
    }
    std::cout << std::endl;
}


void Board::initRoundSpecificState()
{
    for (std::size_t i = 0; i < squares.size(); ++i)
    {
        Piece* piece = squares.at(i).getPiece();
        if (piece)
        {
            piece->resetRoundSpecificState();
        }
    }
}


// private


bool Board::isMoveValid(std::size_t moveFrom, std::size_t moveTo,
    Colour player, unsigned int& mask) const
{
    // TODO do the actual validation
    // Validation should be about going through current states (is checked?),
    // going through selected unit's valid moves, etc.
    if (squares.at(moveFrom).getColourOfPiece() != player)
    {
        mask |= INVALID_TURN;
    }
    Piece* piece = squares.at(moveFrom).getPiece();
    if (piece)
    {
        std::vector<std::size_t> validMoves = getValidMoves(moveFrom);
        bool found = false;
        for (std::size_t i = 0; i < validMoves.size(); ++i)
        {
            if (validMoves.at(i) == moveTo)
            {
                found = true;
                break;
            }
        }

        if (!found)
        {
            mask |= INVALID_MOVE;
        }
    }
    else
    {
        mask |= INVALID_MOVE;
    }

    return mask == 0;
}


bool Board::hasValidMoves(std::size_t location) const
{
    return !(getValidMoves(location).empty());
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


void Board::move(std::size_t moveFrom, std::size_t moveTo)
{
    Piece* piece = squares.at(moveFrom).removePiece();
    squares.at(moveTo).addPiece(piece);
    piece->specialMoveBehaviour(moveFrom, moveTo);
    ++rounds;
}


template <typename T>
void Board::deleteAndClear(std::vector<T>& vector)
{
    while (!vector.empty())
    {
        delete vector.back();
        vector.pop_back();
    }
}

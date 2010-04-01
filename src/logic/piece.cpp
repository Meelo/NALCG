// system includes

// class dependencies
#include "chessboard.h"
#include "piece.h"
#include "square.h"

Piece::Piece(const Piece::Colour& colour, const std::string& name,
    const char symbol) : colour(colour), name(name), symbol(symbol)
{

}

Piece::Colour Piece::getOppositeColour() const
{
    if (colour == Piece::WHITE) return Piece::BLACK;
    if (colour == Piece::BLACK) return Piece::WHITE;
    return Piece::UNDEFINED;
}

bool Piece::isOppositeColour(const Piece::Colour& colour) const
{
    return colour == getOppositeColour();
}

bool Piece::isEmptyOrEdible(std::size_t index, const std::vector<Square>& squares) const
{
    return !squares.at(index).hasPiece() || isOppositeColour(squares.at(index).getColourOfPiece());
}

// By default this function doesn't do anything and that's why we declare
// implement it here, without naming our function's parameters.
void Piece::specialMoveBehaviour(std::size_t, std::size_t) {}

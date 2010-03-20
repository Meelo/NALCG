// system includes

// class dependencies
#include "chessboard.h"
#include "piece.h"
#include "square.h"

Piece::Piece(const Piece::Colour& colour, const std::string& name) :
    colour(colour), name(name)
{

}

std::vector<std::size_t> Piece::getValidMoves(std::size_t location, const std::vector<Square>& squares) const
{
    // Silly unused variables warning.
    // TODO: this method should probable be a pure virtual function.
    if (squares.size() == location) {}
    return std::vector<std::size_t>();
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

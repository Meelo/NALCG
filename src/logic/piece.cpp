// class dependencies
#include "piece.h"

// system includes

Piece::Piece(const Colour& colour, const std::string& name,
    const char symbol) : colour(colour), name(name), symbol(symbol)
{

}

Colour Piece::getOppositeColour() const
{
    if (colour == WHITE) return BLACK;
    if (colour == BLACK) return WHITE;
    return UNDEFINED;
}

bool Piece::isOppositeColour(const Colour& colour) const
{
    return colour == getOppositeColour();
}

bool Piece::isEmptyOrEdible(std::size_t index, const std::vector<Square>& squares) const
{
    return !squares.at(index).hasPiece() || isOppositeColour(squares.at(index).getColourOfPiece());
}


char Piece::getSymbol(bool) const
{
    return symbol;
}

// By default this function doesn't do anything and that's why we declare
// implement it here, without naming our function's parameters.
void Piece::specialMoveBehaviour(std::size_t, std::size_t) {}

bool Piece::trySpecialMove( std::size_t&,   std::size_t&,
                            std::size_t&,   std::size_t&,
                            std::vector<Square>&) const
{
    return false;
}


Colour Piece::getOppositeColour(Colour colour)
{
    if (colour == WHITE) return BLACK;
    if (colour == BLACK) return WHITE;
    return UNDEFINED;
}

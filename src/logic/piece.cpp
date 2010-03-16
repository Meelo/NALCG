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
    return std::vector<std::size_t>();
}

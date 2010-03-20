// system includes

// class dependencies
#include "bishop.h"
#include "chessboard.h"

Bishop::Bishop(const Piece::Colour& colour) : Piece(colour, "Bishop")
{

}

std::vector<std::size_t> Bishop::getValidMoves(std::size_t location, const std::vector<Square>& squares) const
{
    std::vector<std::size_t> validMoves;

    std::size_t diagUpR = location;
    std::size_t diagUpL = location;
    std::size_t diagDownR = location;
    std::size_t diagDownL = location;
    
    if (diagUpR < squares.size() && !squares.at(diagUpR).hasPiece())
    {
        validMoves.push_back(diagUpR);
    }
    
    if (diagUpL < squares.size() && !squares.at(diagUpL).hasPiece())
    {
        validMoves.push_back(diagUpL);
    }
    
    if (diagDownL < squares.size() && !squares.at(diagDownL).hasPiece())
    {
        validMoves.push_back(diagDownL);
    }
    
    if (diagDownR < squares.size() && !squares.at(diagDownR).hasPiece())
    {
        validMoves.push_back(diagDownR);
    }

    return validMoves;
}

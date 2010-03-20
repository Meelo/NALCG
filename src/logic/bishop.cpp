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

    std::size_t x = 0, y = 0;
    ChessBoard::getCoordinates(location, x, y);
    std::size_t diagUpR = ChessBoard::getPosition(x + 1, y - 1);
    std::size_t diagUpL = ChessBoard::getPosition(x - 1, y - 1);
    std::size_t diagDownR = ChessBoard::getPosition(x + 1, y + 1);
    std::size_t diagDownL = ChessBoard::getPosition(x - 1, y + 1);
    
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

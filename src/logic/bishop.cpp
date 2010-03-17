// system includes

// class dependencies
#include "bishop.h"
#include "chessboard.h"

Bishop::Bishop(const Piece::Colour& colour) : Piece(colour, "Bishop"),
    DIAGONAL_MOVE_UP_LEFT(-ChessBoard::WIDTH - 1),
    DIAGONAL_MOVE_UP_RIGHT(-ChessBoard::WIDTH + 1),
    DIAGONAL_MOVE_DOWN_LEFT(ChessBoard::WIDTH - 1),
    DIAGONAL_MOVE_DOWN_RIGHT(ChessBoard::WIDTH + 1)
{

}

std::vector<std::size_t> Bishop::getValidMoves(std::size_t location, const std::vector<Square>& squares) const
{
    std::vector<std::size_t> validMoves;

    std::size_t diagUpR = location + DIAGONAL_MOVE_UP_RIGHT;
    std::size_t diagUpL = location + DIAGONAL_MOVE_UP_LEFT;
    std::size_t diagDownR = location + DIAGONAL_MOVE_DOWN_RIGHT;
    std::size_t diagDownL = location + DIAGONAL_MOVE_DOWN_LEFT;
    
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

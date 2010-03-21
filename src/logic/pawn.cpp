// system includes

// class dependencies
#include "pawn.h"
#include "chessboard.h"

Pawn::Pawn(const Piece::Colour& colour) : Piece(colour, "Pawn")
{

}

std::vector<std::size_t> Pawn::getValidMoves(std::size_t location, 
    const std::vector<Square>& squares) const
{
    std::vector<std::size_t> validMoves;

    int direction = 0;
    if (colour == Piece::WHITE) direction = -1;
    else if (colour == Piece::BLACK) direction = 1;
    else return validMoves;

    std::size_t x = 0, y = 0;
    ChessBoard::getCoordinates(location, x, y);
    std::size_t diagL = ChessBoard::getPosition(x - 1, y + direction);
    std::size_t diagR = ChessBoard::getPosition(x + 1, y + direction);

    if (diagL < squares.size() && 
        isOppositeColour(squares.at(diagL).getColourOfPiece()))
    {
        validMoves.push_back(diagL);
    }
    
    if (diagR < squares.size() && 
        isOppositeColour(squares.at(diagR).getColourOfPiece()))
    {
        validMoves.push_back(diagR);
    }

    std::size_t possibility = ChessBoard::getPosition(x, y + direction);
    if (possibility < squares.size() && !squares.at(possibility).hasPiece())
    {
        validMoves.push_back(possibility);
        
        if ((colour == Piece::WHITE && y == ChessBoard::WHITE_PAWN_ROW) || 
            (colour == Piece::BLACK && y == ChessBoard::BLACK_PAWN_ROW))
        {
            possibility += (direction * ChessBoard::WIDTH);
            if (possibility < squares.size() &&
                !squares.at(possibility).hasPiece())
            {
                validMoves.push_back(possibility);
            }
        }
    }
    
    return validMoves;
}

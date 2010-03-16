// system includes

// class dependencies
#include "pawn.h"

Pawn::Pawn(const Piece::Colour& colour) : Piece(colour, "Pawn")
{

}

std::vector<std::size_t> Pawn::getValidMoves(std::size_t location, const std::vector<Square>& squares) const
{
    std::vector<std::size_t> validMoves;

    int direction = (colour == Piece::WHITE) ? -1 : 1;

    std::size_t y = location / ChessBoard::HEIGHT;
    
    std::size_t possibility = location + (direction * ChessBoard::WIDTH);
    if (possibility < squares.size() && !squares.at(possibility).hasPiece())
    {
        validMoves.push_back(possibility);
        
        if ((colour == Piece::WHITE && y == ChessBoard::WHITE_PAWN_ROW) || (colour == Piece::BLACK && y == ChessBoard::BLACK_PAWN_ROW))
        {
            possibility += (direction * ChessBoard::WIDTH);
            if (possibility < squares.size() && !squares.at(possibility).hasPiece())
            {
                validMoves.push_back(possibility);
            }
        }
    }
    
    return validMoves;
}

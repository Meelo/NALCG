// system includes

// class dependencies
#include "pawn.h"
#include "chessboard.h"

Pawn::Pawn(const Piece::Colour& colour) : Piece(colour, "Pawn",
    colour == Piece::WHITE ? ChessBoard::WHITE_PAWN_SYMBOL : ChessBoard::BLACK_PAWN_SYMBOL),
    specialMoveAllowed(false), resetOnNextTurn(true)
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

    std::size_t left = ChessBoard::getPosition(x - 1, y);
    std::size_t right = ChessBoard::getPosition(x + 1, y);

    if (diagL < squares.size() && (
        isOppositeColour(squares.at(diagL).getColourOfPiece()) ||
        isEnPassantAllowed(diagL, left, squares)) )
    {
        validMoves.push_back(diagL);
    }

    if (diagR < squares.size() && (
        isOppositeColour(squares.at(diagR).getColourOfPiece()) ||
        isEnPassantAllowed(diagR, right, squares)) )
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

void Pawn::specialMoveBehaviour(std::size_t from, std::size_t to)
{
    std::size_t backwards = from + (2 * ChessBoard::WIDTH);
    std::size_t forwards = from - (2 * ChessBoard::WIDTH);
    specialMoveAllowed = backwards == to || forwards == to;
    resetOnNextTurn = false;
}

void Pawn::resetRoundSpecificState()
{
    if (resetOnNextTurn)
    {
        specialMoveAllowed = false;
    }
    resetOnNextTurn = true;
}

// private

bool Pawn::isEnPassantAllowed(std::size_t diag, std::size_t side,
    const std::vector<Square>& squares) const
{
    // Opposite colour pawn
    return (static_cast<char>(squares.at(side).getSymbolOfPiece() ^ (1 << 5)) == symbol
        && squares.at(side).getPiece()->isSpecialMoveAllowed()
        && !squares.at(diag).hasPiece());
}

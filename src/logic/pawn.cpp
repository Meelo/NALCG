// class dependencies
#include "pawn.h"
#include "chessboard.h"

// system includes

Pawn::Pawn(const Colour& colour) : Piece(colour, "Pawn",
    colour == WHITE ? ChessBoard::WHITE_PAWN_SYMBOL : ChessBoard::BLACK_PAWN_SYMBOL),
    specialMoveAllowed(false), resetOnNextTurn(true)
{

}

std::vector<std::size_t> Pawn::getValidMoves(std::size_t location,
    const std::vector<Square>& squares) const
{
    std::vector<std::size_t> validMoves;

    int direction = 0;
    if (colour == WHITE) direction = -1;
    else if (colour == BLACK) direction = 1;
    else return validMoves;

    std::size_t x = 0, y = 0;
    ChessBoard::getCoordinates(location, x, y);
    std::size_t diagL = ChessBoard::getPosition(x - 1, y + direction);
    std::size_t diagR = ChessBoard::getPosition(x + 1, y + direction);

    std::size_t left = ChessBoard::getPosition(x - 1, y);
    std::size_t right = ChessBoard::getPosition(x + 1, y);

    if (isEatingAllowed(diagL, left, squares))
    {
        validMoves.push_back(diagL);
    }

    if (isEatingAllowed(diagR, right, squares))
    {
        validMoves.push_back(diagR);
    }

    std::size_t possibility = ChessBoard::getPosition(x, y + direction);
    if (isMoveAllowed(possibility, squares))
    {
        validMoves.push_back(possibility);

        if (isDoubleMoveAllowed(y))
        {
            possibility += (direction * ChessBoard::WIDTH);
            if (isMoveAllowed(possibility, squares))
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

bool Pawn::isEatingAllowed(std::size_t target, std::size_t side,
    const std::vector<Square>& squares) const
{
    return (target < squares.size() &&
        (isOppositeColour(squares.at(target).getColourOfPiece()) ||
        isEnPassantAllowed(target, side, squares)));
}

bool Pawn::isEnPassantAllowed(std::size_t diag, std::size_t side,
    const std::vector<Square>& squares) const
{
    // Make sure your pawn is right next to opposite colour pawn,
    // since en passant move is only allowed to be done over pawns.
    return (static_cast<char>(squares.at(side).getSymbolOfPiece() ^ (1 << 5)) == symbol
        && squares.at(side).getPiece()->isSpecialMoveAllowed()
        && !squares.at(diag).hasPiece());
}

bool Pawn::isMoveAllowed(std::size_t location,
    const std::vector<Square>& squares) const
{
    return location < squares.size() && !squares.at(location).hasPiece();
}

bool Pawn::isDoubleMoveAllowed(std::size_t row) const
{
    return  (colour == WHITE && row == ChessBoard::WHITE_PAWN_ROW) ||
            (colour == BLACK && row == ChessBoard::BLACK_PAWN_ROW);
}

bool Pawn::trySpecialMove(  std::size_t& fromX, std::size_t& fromY,
                            std::size_t& toX,   std::size_t& toY,
                            std::vector<Square>& squares) const
{
    if (fromX != toX && fromY != toY)
    {
        std::size_t target =  ChessBoard::getPosition(toX, toY);
        std::size_t side = ChessBoard::getPosition(toX, fromY);
        if (isEnPassantAllowed(target, side, squares))
        {
            squares.at(target).addPiece(squares.at(side).removePiece());
            fromX = toX;
            return true;
        }
    }
    return false;
}

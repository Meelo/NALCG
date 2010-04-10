// class dependencies
#include "king.h"
#include "chessboard.h"

// system includes

const int King::X_DIRECTIONS[] = { 1, -1, 0,  0, -1, -1, 1,  1, 0 };
const int King::Y_DIRECTIONS[] = { 0,  0, 1, -1,  1, -1, 1, -1, 0 };
const int King::CASTLING = 2;
const int King::LEFT_ROOK_X = 0;
const int King::RIGHT_ROOK_X = ChessBoard::WIDTH - 1;

King::King(const Colour& colour) : Piece(colour, "King",
    colour == WHITE ? ChessBoard::WHITE_KING_SYMBOL : ChessBoard::BLACK_KING_SYMBOL),
    specialMoveAllowed(true)
{

}

std::vector<std::size_t> King::getValidMoves(std::size_t ownLocation,
    const std::vector<Square>& squares, std::size_t protect) const
{
    std::vector<std::size_t> validMoves;
    std::size_t location = ownLocation;

    std::size_t x = 0, y = 0;
    ChessBoard::getCoordinates(location, x, y);

    for (std::size_t i = 0; X_DIRECTIONS[i] != 0 || Y_DIRECTIONS[i] != 0; ++i)
    {
        std::size_t location = ChessBoard::getPosition(x + X_DIRECTIONS[i],
            y + Y_DIRECTIONS[i]);

        if (location < squares.size() && isEmptyOrEdible(location, squares) &&
            !ChessBoard::isUnderAttack(ownLocation, squares, ownLocation, location))
        {
            validMoves.push_back(location);

            std::size_t x1 = x, y1 = y;
            ChessBoard::getCoordinates(location, x1, y1);
            location = ChessBoard::getPosition(x1 + X_DIRECTIONS[i],
                y1 + Y_DIRECTIONS[i]);
        }
    }

    if (specialMoveAllowed &&
        !ChessBoard::isUnderAttack(ownLocation, squares))
    {
        ChessBoard::getCoordinates(location, x, y);

        // check for the rook on the left side
        std::size_t leftRook = ChessBoard::getPosition(LEFT_ROOK_X, y);
        if (isCastlingAllowed(leftRook, squares) &&
            !ChessBoard::isUnderAttack(leftRook, squares))
        {
            bool castlingPossible = true;
            for (std::size_t i = leftRook + 1; i < location; ++i) {
                if (squares.at(i).hasPiece() ||
                    ChessBoard::isUnderAttack(ownLocation, squares, ownLocation, i))
                {
                    castlingPossible = false;
                    break;
                }
            }
            if (castlingPossible)
            {
                validMoves.push_back(location - CASTLING);
            }
        }

        // check for the rook on the right side
        std::size_t rightRook = ChessBoard::getPosition(RIGHT_ROOK_X, y);
        if (isCastlingAllowed(rightRook, squares) &&
            !ChessBoard::isUnderAttack(rightRook, squares))
        {
            bool castlingPossible = true;
            for (std::size_t i = location + 1; i < rightRook; ++i) {
                if (squares.at(i).hasPiece() ||
                    ChessBoard::isUnderAttack(ownLocation, squares, ownLocation, i))
                {
                    castlingPossible = false;
                    break;
                }
            }
            if (castlingPossible)
            {
                validMoves.push_back(location + CASTLING);
            }
        }
    }

    return validMoves;
}

void King::specialMoveBehaviour(std::size_t, std::size_t)
{
    specialMoveAllowed = false;
}

// private

bool King::isCastlingAllowed(std::size_t rookLocation,
    const std::vector<Square>& squares) const
{
    return colour == squares.at(rookLocation).getColourOfPiece() &&
        squares.at(rookLocation).getPiece()->isSpecialMoveAllowed();
}

bool King::trySpecialMove(  std::size_t& fromX, std::size_t& fromY,
                            std::size_t& toX,   std::size_t& toY,
                            std::vector<Square>& squares) const
{
    int moveLength = fromX - toX;

    if (moveLength == CASTLING)
    {
        fromX = LEFT_ROOK_X;
        toX++;
    }
    else if (moveLength == -CASTLING)
    {
        fromX = RIGHT_ROOK_X;
        toX--;
    }
    else
    {
        return false;
    }
    std::size_t moveFrom = ChessBoard::getPosition(fromX, fromY);
    std::size_t moveTo = ChessBoard::getPosition(toX, toY);
    Piece* rook = squares.at(moveFrom).removePiece();
    squares.at(moveTo).addPiece(rook);
    rook->specialMoveBehaviour(moveFrom, moveTo);

    return true;
}

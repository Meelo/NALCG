// system includes

// class dependencies
#include "king.h"
#include "chessboard.h"

const int King::X_DIRECTIONS[] = { 1, -1, 0,  0, -1, -1, 1,  1, 0 };
const int King::Y_DIRECTIONS[] = { 0,  0, 1, -1,  1, -1, 1, -1, 0 };
const int King::CASTLING = 2;
const int King::LEFT_ROOK_X = 0;
const int King::RIGHT_ROOK_X = ChessBoard::WIDTH - 1;

King::King(const Piece::Colour& colour) : Piece(colour, "King",
    colour == Piece::WHITE ? ChessBoard::WHITE_KING_SYMBOL : ChessBoard::BLACK_KING_SYMBOL),
    specialMoveAllowed(true)
{

}

std::vector<std::size_t> King::getValidMoves(std::size_t location,
    const std::vector<Square>& squares) const
{
    std::vector<std::size_t> validMoves;

    std::size_t x = 0, y = 0;
    ChessBoard::getCoordinates(location, x, y);

    for (std::size_t i = 0; X_DIRECTIONS[i] != 0 || Y_DIRECTIONS[i] != 0; ++i)
    {
        std::size_t location = ChessBoard::getPosition(x + X_DIRECTIONS[i],
            y + Y_DIRECTIONS[i]);

        if (location < squares.size() && isEmptyOrEdible(location, squares))
        {
            validMoves.push_back(location);

            std::size_t x1 = x, y1 = y;
            ChessBoard::getCoordinates(location, x1, y1);
            location = ChessBoard::getPosition(x1 + X_DIRECTIONS[i],
                y1 + Y_DIRECTIONS[i]);
        }
    }

    if (specialMoveAllowed) {
        ChessBoard::getCoordinates(location, x, y);

        // check for the rook on the left side
        std::size_t leftRook = ChessBoard::getPosition(LEFT_ROOK_X, y);
        if (colour == squares.at(leftRook).getColourOfPiece() &&
            squares.at(leftRook).getPiece()->isSpecialMoveAllowed())
        {
            bool castlingAllowed = true;
            for (std::size_t i = leftRook + 1; i < location; ++i) {
                if (squares.at(i).hasPiece())
                {
                    castlingAllowed = false;
                    break;
                }
            }
            if (castlingAllowed)
            {
                validMoves.push_back(location - CASTLING);
            }
        }

        // check for the rook on the right side
        std::size_t rightRook = ChessBoard::getPosition(RIGHT_ROOK_X, y);
        if (colour == squares.at(rightRook).getColourOfPiece() &&
            squares.at(rightRook).getPiece()->isSpecialMoveAllowed())
        {
            bool castlingAllowed = true;
            for (std::size_t i = location + 1; i < rightRook; ++i) {
                if (squares.at(i).hasPiece())
                {
                    castlingAllowed = false;
                    break;
                }
            }
            if (castlingAllowed)
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

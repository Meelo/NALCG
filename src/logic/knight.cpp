// class dependencies
#include "knight.h"
#include "chessboard.h"

// system includes

const int Knight::X_DIRECTIONS[] = { -1,  1, -2,  2, -1, 1, -2, 2, 0 };
const int Knight::Y_DIRECTIONS[] = { -2, -2, -1, -1,  2, 2,  1, 1, 0 };

Knight::Knight(const Colour& colour) : Piece(colour, "Knight",
    colour == WHITE ? ChessBoard::WHITE_KNIGHT_SYMBOL : ChessBoard::BLACK_KNIGHT_SYMBOL)
{

}

std::vector<std::size_t> Knight::getValidMoves(std::size_t ownLocation,
    const std::vector<Square>& squares, std::size_t protect) const
{
    std::vector<std::size_t> validMoves;
    std::size_t location = ownLocation;
    std::size_t limit = squares.size();

    std::size_t x = 0, y = 0;
    ChessBoard::getCoordinates(location, x, y);

    for (std::size_t i = 0; (X_DIRECTIONS[i] != 0 || Y_DIRECTIONS[i] != 0); ++i)
    {
        std::size_t location = ChessBoard::getPosition(x + X_DIRECTIONS[i],
            y + Y_DIRECTIONS[i]);

        if (location < limit && isEmptyOrEdible(location, squares) &&
            !ChessBoard::isUnderAttack(protect, squares, ownLocation, location))
        {
            validMoves.push_back(location);
        }
    }

    return validMoves;
}

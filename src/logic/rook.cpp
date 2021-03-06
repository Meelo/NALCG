// class dependencies
#include "rook.h"
#include "chessboard.h"

// system includes

const int Rook::X_DIRECTIONS[] = { 1, -1, 0,  0, 0 };
const int Rook::Y_DIRECTIONS[] = { 0,  0, 1, -1, 0 };

Rook::Rook(const Colour& colour) : Piece(colour, "Rook",
    colour == WHITE ? ChessBoard::WHITE_ROOK_SYMBOL : ChessBoard::BLACK_ROOK_SYMBOL),
    specialMoveAllowed(true)
{

}

std::vector<std::size_t> Rook::getValidMoves(std::size_t ownLocation,
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

        while (location < limit && isEmptyOrEdible(location, squares))
        {
            if (!ChessBoard::isUnderAttack(protect, squares, ownLocation, location))
            {
                validMoves.push_back(location);
            }

            // Don't go any further when edible target has been found.
            if (isOppositeColour(squares.at(location).getColourOfPiece())) break;

            std::size_t x1 = x, y1 = y;
            ChessBoard::getCoordinates(location, x1, y1);
            location = ChessBoard::getPosition(x1 + X_DIRECTIONS[i],
                y1 + Y_DIRECTIONS[i]);

        }
    }

    return validMoves;
}


void Rook::specialMoveBehaviour(std::size_t, std::size_t)
{
    specialMoveAllowed = false;
}


char Rook::getSymbol(bool markSpecialSymbols) const
{
    if (markSpecialSymbols && specialMoveAllowed)
    {
        // is white symbol? (white is lowercase, black is uppercase)
        if (symbol & (1 << 5))
        {
            return ChessBoard::WHITE_ROOK_SYMBOL_SPECIAL;
        }
        else
        {
            return ChessBoard::BLACK_ROOK_SYMBOL_SPECIAL;
        }
    }

    return symbol;
}

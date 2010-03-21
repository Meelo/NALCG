// system includes

// class dependencies
#include "king.h"
#include "chessboard.h"

const int King::X_DIRECTIONS[] = { 1, -1, 0,  0, -1, -1, 1,  1, 0 };
const int King::Y_DIRECTIONS[] = { 0,  0, 1, -1,  1, -1, 1, -1, 0 };

King::King(const Piece::Colour& colour) : Piece(colour, "King")
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
            if (isOppositeColour(squares.at(location).getColourOfPiece())) break;

            std::size_t x1 = x, y1 = y;
            ChessBoard::getCoordinates(location, x1, y1);
            location = ChessBoard::getPosition(x1 + X_DIRECTIONS[i], 
                y1 + Y_DIRECTIONS[i]);
        }
    }
    
    return validMoves;
}


// system includes

// class dependencies
#include "chessboard.h"

ChessBoard::ChessBoard() :
    Board(WIDTH, HEIGHT)
{

}

std::vector<Square> ChessBoard::createBoard()
{
    std::vector<Square> squares;
    for (std::size_t i = 0; i < BOARD_SIZE; ++i)
    {
        Square square;
        squares.push_back(square);
    }

    return squares;
}

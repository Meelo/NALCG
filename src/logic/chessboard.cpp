// system includes

// class dependencies
#include "chessboard.h"
#include "piece.h"
#include "pawn.h"
#include "rook.h"
#include "knight.h"
//#include "bishop.h"
//#include "queen.h"
//#include "king.h"

ChessBoard::ChessBoard() :
    Board(WIDTH, HEIGHT)
{

}

// Chess board looks like this
//
//    A  B  C  D  E  F  G  H
// 8 |br|bk|bb|bq|bK|bb|bk|br| 8
// 7 |bp|bp|bp|bp|bp|bp|bp|bp| 7
// 6 |  |  |  |  |  |  |  |  | 6
// 5 |  |  |  |  |  |  |  |  | 5
// 4 |  |  |  |  |  |  |  |  | 4
// 3 |  |  |  |  |  |  |  |  | 3
// 2 |wp|wp|wp|wp|wp|wp|wp|wp| 2
// 1 |wr|wk|wb|wq|wK|wb|wk|wr| 1
//    A  B  C  D  E  F  G  H
//
// br = black rook, bk = black knight, bb = black bishop
// bq = black queen, bK = black king, bp = black pawn
// wr = white rook, wk = white knight, wb = white bishop
// wq = white queen, wK = white king, wp = white pawn
//
// Therefore the numbering in one dimensional array goes like this:
//    A  B  C  D  E  F  G  H
// 8 |00|01|02|03|04|05|06|07| 8
// 7 |08|09|10|11|12|13|14|15| 7
// 6 |16|17|18|19|20|21|22|23| 6
// 5 |24|25|26|27|28|29|30|31| 5
// 4 |32|33|34|35|36|37|38|39| 4
// 3 |40|41|42|43|44|45|46|47| 3
// 2 |48|49|50|51|52|53|54|55| 2
// 1 |56|57|58|59|60|61|62|63| 1
//    A  B  C  D  E  F  G  H
std::vector<Square> ChessBoard::createBoard()
{
    Piece::Colour black = Piece::BLACK;
    Piece::Colour white = Piece::WHITE;
    
    std::vector<Square> squares;
    
    // The magic numbers are the row indices, from top to bottom.
    std::size_t blackPawnRow = 1 * WIDTH;
    std::size_t whitePawnRow = 6 * WIDTH;

    for (std::size_t i = 0; i < BOARD_SIZE; ++i)
    {
        Square square;
        squares.push_back(square);
    }

    for (std::size_t column = 0; column < WIDTH; ++column)
    {
        squares.at(blackPawnRow + column).addPiece(new Pawn(black));
        squares.at(whitePawnRow + column).addPiece(new Pawn(white));
    }

    // Rooks
    squares.at(BLACK_ROOK_LEFT).addPiece(new Rook(black));
    squares.at(BLACK_ROOK_RIGHT).addPiece(new Rook(black));
    squares.at(WHITE_ROOK_LEFT).addPiece(new Rook(white));
    squares.at(WHITE_ROOK_RIGHT).addPiece(new Rook(white));

    // Knights
    squares.at(BLACK_KNIGHT_LEFT).addPiece(new Knight(black));
    squares.at(BLACK_KNIGHT_RIGHT).addPiece(new Knight(black));
    squares.at(WHITE_KNIGHT_LEFT).addPiece(new Knight(white));
    squares.at(WHITE_KNIGHT_RIGHT).addPiece(new Knight(white));
    
    // Bishops
    // Queens
    // Kings

    return squares;
}

// system includes

// class dependencies
#include "chessboard.h"
#include "chesspieces-meta.h"

// Constant definitions
const std::size_t ChessBoard::WIDTH = 8;
const std::size_t ChessBoard::HEIGHT = 8;
const std::size_t ChessBoard::BOARD_SIZE = 64;
const std::size_t ChessBoard::BLACK_PAWN_ROW = 1;
const std::size_t ChessBoard::WHITE_PAWN_ROW = 6;
// default positions - black
const std::size_t ChessBoard::BLACK_ROOK_LEFT = 0;
const std::size_t ChessBoard::BLACK_KNIGHT_LEFT = 1;
const std::size_t ChessBoard::BLACK_BISHOP_LEFT = 2;
const std::size_t ChessBoard::BLACK_QUEEN = 3;
const std::size_t ChessBoard::BLACK_KING = 4;
const std::size_t ChessBoard::BLACK_BISHOP_RIGHT = 5;
const std::size_t ChessBoard::BLACK_KNIGHT_RIGHT = 6;
const std::size_t ChessBoard::BLACK_ROOK_RIGHT = 7;
// default positions - white
const std::size_t ChessBoard::WHITE_ROOK_LEFT = 56;
const std::size_t ChessBoard::WHITE_KNIGHT_LEFT = 57;
const std::size_t ChessBoard::WHITE_BISHOP_LEFT = 58;
const std::size_t ChessBoard::WHITE_QUEEN = 59;
const std::size_t ChessBoard::WHITE_KING = 60;
const std::size_t ChessBoard::WHITE_BISHOP_RIGHT = 61;
const std::size_t ChessBoard::WHITE_KNIGHT_RIGHT = 62;
const std::size_t ChessBoard::WHITE_ROOK_RIGHT = 63;
// piece symbols - black
const char ChessBoard::BLACK_PAWN_SYMBOL = 'P';
const char ChessBoard::BLACK_ROOK_SYMBOL = 'R';
const char ChessBoard::BLACK_KNIGHT_SYMBOL = 'N';
const char ChessBoard::BLACK_BISHOP_SYMBOL = 'B';
const char ChessBoard::BLACK_QUEEN_SYMBOL = 'Q';
const char ChessBoard::BLACK_KING_SYMBOL = 'K';
// piece symbols - white
const char ChessBoard::WHITE_PAWN_SYMBOL = 'p';
const char ChessBoard::WHITE_ROOK_SYMBOL = 'r';
const char ChessBoard::WHITE_KNIGHT_SYMBOL = 'n';
const char ChessBoard::WHITE_BISHOP_SYMBOL = 'b';
const char ChessBoard::WHITE_QUEEN_SYMBOL = 'q';
const char ChessBoard::WHITE_KING_SYMBOL = 'k';

ChessBoard::ChessBoard(const std::vector<Square>& squares) :
    Board(squares, WIDTH, HEIGHT)
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
    std::size_t blackPawnRow = BLACK_PAWN_ROW * WIDTH;
    std::size_t whitePawnRow = WHITE_PAWN_ROW * WIDTH;

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
    squares.at(BLACK_BISHOP_LEFT).addPiece(new Bishop(black));
    squares.at(BLACK_BISHOP_RIGHT).addPiece(new Bishop(black));
    squares.at(WHITE_BISHOP_LEFT).addPiece(new Bishop(white));
    squares.at(WHITE_BISHOP_RIGHT).addPiece(new Bishop(white));

    // Queens
    squares.at(BLACK_QUEEN).addPiece(new Queen(black));
    squares.at(WHITE_QUEEN).addPiece(new Queen(white));

    // Kings
    squares.at(BLACK_KING).addPiece(new King(black));
    squares.at(WHITE_KING).addPiece(new King(white));

    return squares;
}

bool ChessBoard::getCoordinates(std::size_t index, std::size_t& column,
    std::size_t& row)
{
    return Board::getCoordinates(index, column, row, WIDTH, HEIGHT);
}

std::size_t ChessBoard::getPosition(std::size_t column, std::size_t row)
{
    return Board::getPosition(column, row, WIDTH, HEIGHT);
}


// class dependencies
#include "chessboard.h"
#include "chesspieces-meta.h"

// system includes

// Constant definitions
const std::size_t ChessBoard::WIDTH                 = 8;
const std::size_t ChessBoard::HEIGHT                = 8;
const std::size_t ChessBoard::BOARD_SIZE            = 64;
const std::size_t ChessBoard::BLACK_PAWN_ROW        = 1;
const std::size_t ChessBoard::WHITE_PAWN_ROW        = 6;
const std::size_t ChessBoard::BLACK_PROMOTE_ROW     = 7;
const std::size_t ChessBoard::WHITE_PROMOTE_ROW     = 0;

const unsigned int ChessBoard::PROMOTE_TO_BISHOP    = 1;
const unsigned int ChessBoard::PROMOTE_TO_ROOK      = 2;
const unsigned int ChessBoard::PROMOTE_TO_KNIGHT    = 3;
const unsigned int ChessBoard::PROMOTE_TO_QUEEN     = 4;

// default positions - black
const std::size_t ChessBoard::BLACK_ROOK_LEFT       = 0;
const std::size_t ChessBoard::BLACK_KNIGHT_LEFT     = 1;
const std::size_t ChessBoard::BLACK_BISHOP_LEFT     = 2;
const std::size_t ChessBoard::BLACK_QUEEN           = 3;
const std::size_t ChessBoard::BLACK_KING            = 4;
const std::size_t ChessBoard::BLACK_BISHOP_RIGHT    = 5;
const std::size_t ChessBoard::BLACK_KNIGHT_RIGHT    = 6;
const std::size_t ChessBoard::BLACK_ROOK_RIGHT      = 7;
// default positions - white
const std::size_t ChessBoard::WHITE_ROOK_LEFT       = 56;
const std::size_t ChessBoard::WHITE_KNIGHT_LEFT     = 57;
const std::size_t ChessBoard::WHITE_BISHOP_LEFT     = 58;
const std::size_t ChessBoard::WHITE_QUEEN           = 59;
const std::size_t ChessBoard::WHITE_KING            = 60;
const std::size_t ChessBoard::WHITE_BISHOP_RIGHT    = 61;
const std::size_t ChessBoard::WHITE_KNIGHT_RIGHT    = 62;
const std::size_t ChessBoard::WHITE_ROOK_RIGHT      = 63;
// piece symbols - black
const char ChessBoard::BLACK_PAWN_SYMBOL            = 'P';
const char ChessBoard::BLACK_ROOK_SYMBOL            = 'R';
const char ChessBoard::BLACK_KNIGHT_SYMBOL          = 'N';
const char ChessBoard::BLACK_BISHOP_SYMBOL          = 'B';
const char ChessBoard::BLACK_QUEEN_SYMBOL           = 'Q';
const char ChessBoard::BLACK_KING_SYMBOL            = 'K';
// piece symbols - white
const char ChessBoard::WHITE_PAWN_SYMBOL            = 'p';
const char ChessBoard::WHITE_ROOK_SYMBOL            = 'r';
const char ChessBoard::WHITE_KNIGHT_SYMBOL          = 'n';
const char ChessBoard::WHITE_BISHOP_SYMBOL          = 'b';
const char ChessBoard::WHITE_QUEEN_SYMBOL           = 'q';
const char ChessBoard::WHITE_KING_SYMBOL            = 'k';

const char ChessBoard::EMPTY_SYMBOL                 = 0;


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
    Colour black = BLACK;
    Colour white = WHITE;

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


// static


//
// Check if piece in 'location' is threatened by enemy pieces.
// You can emulate a movement by passing moveFrom and moveTo -parameters.
//
// If moveFrom-parameter is larger than squares.size(),
// both moveFrom and moveTo -parameters will be ignored.
//
// If both moveFrom and moveTo -parameters are the same,
// a piece will be ignored at 'moveFrom'-location.
//
// If location and moveFrom parameters are the same,
// moveTo-parameter will be used for protecting purposes.
//
bool ChessBoard::isUnderAttack(std::size_t location,
    const std::vector<Square>& squares,
    std::size_t moveFrom, std::size_t moveTo)
{
    std::size_t limit = squares.size();
    if (location >= limit || !squares.at(location).hasPiece())
    {
        return false;
    }
    char symbol = squares.at(location).getSymbolOfPiece();
    if (moveFrom >= limit)
    {
        // No piece shall be ignored or 'moved'.
        moveFrom = ~0;
        moveTo = ~0;
    }
    else if (moveFrom == moveTo)
    {
        // Only moveFrom shall be used.
        moveTo = ~0;
    }
    else if (location == moveFrom)
    {
        location = moveTo;
        moveTo = ~0;
    }

    return  isRectanglyUnsafe   (location, symbol, squares, moveFrom, moveTo) ||
            isDiagonallyUnsafe  (location, symbol, squares, moveFrom, moveTo) ||
            isPawnlyUnsafe      (location, symbol, squares, moveFrom, moveTo) ||
            isKnightlyUnsafe    (location, symbol, squares, moveFrom, moveTo) ||
            isKinglyUnsafe      (location, symbol, squares, moveFrom, moveTo);
}


std::vector<std::size_t> ChessBoard::getValidMoves(std::size_t location) const
{
    // TODO: cache these results per each turn.
    if (location >= squares.size()) { return std::vector<std::size_t>(); }
    Piece* piece = squares.at(location).getPiece();
    if (!piece) { return std::vector<std::size_t>(); }

    Colour colour = piece->getColour();
    std::size_t kingLocation = findKing(colour);
    if (kingLocation >= squares.size()) { return std::vector<std::size_t>(); }

    return piece->getValidMoves(location, squares, kingLocation);
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


// private


bool ChessBoard::isPromotable(std::size_t moveFrom, std::size_t moveTo) const
{
    const char pieceSymbol = squares.at(moveFrom).getSymbolOfPiece();
    std::size_t x = 0, y = 0;
    getCoordinates(moveTo, x, y);
    return ((pieceSymbol == WHITE_PAWN_SYMBOL && y == WHITE_PROMOTE_ROW) ||
            (pieceSymbol == BLACK_PAWN_SYMBOL && y == BLACK_PROMOTE_ROW));
}


void ChessBoard::promote(std::size_t location, unsigned int promoteTo)
{
    Piece* newPiece = 0;
    Colour colour = squares.at(location).getColourOfPiece();
    switch (promoteTo)
    {
        case PROMOTE_TO_BISHOP:
            newPiece = new Bishop(colour);
            break;
        case PROMOTE_TO_ROOK:
            newPiece = new Rook(colour);
            break;
        case PROMOTE_TO_KNIGHT:
            newPiece = new Knight(colour);
            break;
        case PROMOTE_TO_QUEEN:
        default:
            newPiece = new Queen(colour);
            break;
    }
    if (newPiece)
    {
        delete squares.at(location).removePiece();
        squares.at(location).addPiece(newPiece);
    }
}


std::size_t ChessBoard::findKing(Colour colour) const
{
    char kingSymbol = (colour == WHITE) ? WHITE_KING_SYMBOL : BLACK_KING_SYMBOL;

    for (std::size_t i = 0; i < squares.size(); ++i)
    {
        if (squares.at(i).getSymbolOfPiece() == kingSymbol)
        {
            // king found, return its index.
            return i;
        }
    }

    // king not found, return out of bounds value.
    return squares.size();
}


bool ChessBoard::isBishop(char symbol)
{
    // symbol | (1 << 5) lowercases the symbol if it was uppercase.
    return ((symbol | (1 << 5)) == WHITE_BISHOP_SYMBOL);
}


bool ChessBoard::isRook(char symbol)
{
    // symbol | (1 << 5) lowercases the symbol if it was uppercase.
    return ((symbol | (1 << 5)) == WHITE_ROOK_SYMBOL);
}


bool ChessBoard::isKnight(char symbol)
{
    // symbol | (1 << 5) lowercases the symbol if it was uppercase.
    return ((symbol | (1 << 5)) == WHITE_KNIGHT_SYMBOL);
}


bool ChessBoard::isQueen(char symbol)
{
    // symbol | (1 << 5) lowercases the symbol if it was uppercase.
    return ((symbol | (1 << 5)) == WHITE_QUEEN_SYMBOL);
}


bool ChessBoard::areOppositeColour(char symbol1, char symbol2)
{
    char toUpperCaseBit = 1 << 5;
    // if toUpperCaseBit is set or unset in both, then they are the same colour
    // otherwise they are opposite colours.
    return ((symbol1 & toUpperCaseBit) != (symbol2 & toUpperCaseBit));
}


bool ChessBoard::isRectanglyUnsafe(std::size_t location, char symbol,
    const std::vector<Square>& squares, std::size_t moveFrom,
    std::size_t moveTo)
{
    std::size_t x = 0, y = 0;
    getCoordinates(location, x, y);
    std::size_t limit = squares.size();

    for (std::size_t i = 0;
        (RECTANGLE_DIRECTIONS_X[i] != 0 || RECTANGLE_DIRECTIONS_Y[i] != 0);
        ++i)
    {
        std::size_t newLocation = getPosition(x + RECTANGLE_DIRECTIONS_X[i],
            y + RECTANGLE_DIRECTIONS_Y[i]);

        while (newLocation < limit )
        {
            if (newLocation == moveTo) { break; }
            char currentPiece = squares.at(newLocation).getSymbolOfPiece();
            if (newLocation != moveFrom && currentPiece != EMPTY_SYMBOL)
            {
                if (!areOppositeColour(symbol, currentPiece))
                {
                    // Found allied
                    break;
                }
                else if (isRook(currentPiece) || isQueen(currentPiece))
                {
                    // found opponent rook or queen.
                    return true;
                }
                else
                {
                    // found opponent which can't attack rectangly.
                    break;
                }
            }

            std::size_t x1 = x, y1 = y;
            getCoordinates(newLocation, x1, y1);
            newLocation = getPosition(x1 + RECTANGLE_DIRECTIONS_X[i],
                y1 + RECTANGLE_DIRECTIONS_Y[i]);
        }
    }

    return false;
}


bool ChessBoard::isDiagonallyUnsafe(std::size_t location, char symbol,
    const std::vector<Square>& squares, std::size_t moveFrom,
    std::size_t moveTo)
{
    std::size_t x = 0, y = 0;
    getCoordinates(location, x, y);
    std::size_t limit = squares.size();

    for (std::size_t i = 0;
        (DIAGONAL_DIRECTIONS_X[i] != 0 || DIAGONAL_DIRECTIONS_Y[i] != 0);
        ++i)
    {
        std::size_t newLocation = getPosition(x + DIAGONAL_DIRECTIONS_X[i],
            y + DIAGONAL_DIRECTIONS_Y[i]);

        while (newLocation < limit )
        {

            if (newLocation == moveTo) { break; }
            char currentPiece = squares.at(newLocation).getSymbolOfPiece();
            if (newLocation != moveFrom && currentPiece != EMPTY_SYMBOL)
            {
                if (!areOppositeColour(symbol, currentPiece))
                {
                    // Found allied
                    break;
                }
                else if (isBishop(currentPiece) || isQueen(currentPiece))
                {
                    // found opponent bishop or queen.
                    return true;
                }
                else
                {
                    // found opponent which can't attack diagonally.
                    break;
                }
            }

            std::size_t x1 = x, y1 = y;
            getCoordinates(newLocation, x1, y1);
            newLocation = getPosition(x1 + DIAGONAL_DIRECTIONS_X[i],
                y1 + DIAGONAL_DIRECTIONS_Y[i]);
        }
    }

    return false;
}


bool ChessBoard::isKinglyUnsafe(std::size_t location, char symbol,
    const std::vector<Square>& squares, std::size_t moveFrom,
    std::size_t moveTo)
{
    std::size_t x = 0, y = 0;
    getCoordinates(location, x, y);

    return false;
}


bool ChessBoard::isKnightlyUnsafe(std::size_t location, char symbol,
    const std::vector<Square>& squares, std::size_t moveFrom,
    std::size_t moveTo)
{
    std::size_t x = 0, y = 0;
    getCoordinates(location, x, y);
    std::size_t limit = squares.size();

    for (std::size_t i = 0;
        (KNIGHT_DIRECTIONS_X[i] != 0 || KNIGHT_DIRECTIONS_Y[i] != 0);
        ++i)
    {
        std::size_t newLocation = getPosition(x + KNIGHT_DIRECTIONS_X[i],
            y + KNIGHT_DIRECTIONS_Y[i]);

        if (newLocation < limit )
        {
            char currentPiece = squares.at(newLocation).getSymbolOfPiece();
            if (newLocation == moveFrom || newLocation == moveTo)
            {
                continue;
            }
            else if (areOppositeColour(symbol, currentPiece) &&
                isKnight(currentPiece))
            {
                // found enemy knight.
                return true;
            }
        }
    }

    return false;
}


bool ChessBoard::isPawnlyUnsafe(std::size_t location, char symbol,
    const std::vector<Square>& squares, std::size_t moveFrom,
    std::size_t moveTo)
{
    std::size_t x = 0, y = 0;
    getCoordinates(location, x, y);

    return false;
}

// private constants
const int ChessBoard::RECTANGLE_DIRECTIONS_X[] = { 1, -1, 0,  0, 0 };
const int ChessBoard::RECTANGLE_DIRECTIONS_Y[] = { 0,  0, 1, -1, 0 };
const int ChessBoard::DIAGONAL_DIRECTIONS_X[] = {  1, -1, 1, -1, 0 };
const int ChessBoard::DIAGONAL_DIRECTIONS_Y[] = { -1, -1, 1,  1, 0 };
const int ChessBoard::KNIGHT_DIRECTIONS_X[] = { -1,  1, -2,  2, -1, 1, -2, 2, 0 };
const int ChessBoard::KNIGHT_DIRECTIONS_Y[] = { -2, -2, -1, -1,  2, 2,  1, 1, 0 };

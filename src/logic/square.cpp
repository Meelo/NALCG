// class dependencies
#include "square.h"
#include "piece.h"

// system includes

Square::Square(Piece* piece) : piece(piece)
{

}

Square::Square(const Square& orig)
{
    piece = orig.piece ? orig.piece->clone() : 0;
}

const std::string Square::getNameOfPiece() const
{
    if (piece)
    {
        return piece->getName();
    }

    return "";
}

Colour Square::getColourOfPiece() const
{
    if (piece)
    {
        return piece->getColour();
    }

    return UNDEFINED;
}

char Square::getSymbolOfPiece(bool markSpecialSymbols) const
{
    if (piece)
    {
        return piece->getSymbol(markSpecialSymbols);
    }

    return 0;
}

void Square::addPiece(Piece* newPiece)
{
    piece = newPiece;
}

Piece* Square::removePiece()
{
    Piece* _piece = piece;
    piece = 0;

    return _piece;
}

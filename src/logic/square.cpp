// system includes

// class dependencies
#include "square.h"

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

Piece::Colour Square::getColourOfPiece() const
{
    if (piece)
    {
        return piece->getColour();
    }

    return Piece::UNDEFINED;
}

char Square::getSymbolOfPiece() const
{
    if (piece)
    {
        return piece->getSymbol();
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

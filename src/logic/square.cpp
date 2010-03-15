// system includes

// class dependencies
#include "square.h"

Square::Square(Piece* piece) : piece(piece)
{

}


const std::string Square::getNameOfPiece() const
{
    if (piece)
    {
        return piece->getName();
    }

    return "empty";
}

Piece::Colour Square::getColourOfPiece() const
{
    if (piece)
    {
        return piece->getColour();
    }

    return Piece::UNDEFINED;
}


Piece* Square::removePiece()
{
    Piece* _piece = piece;
    piece = 0;

    return _piece;
}

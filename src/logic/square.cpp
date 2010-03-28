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

    return ' ';
}

void Square::addPiece(Piece* newPiece)
{
    // TODO: All pieces should be in a container which is emptied in
    // the end of game. This is because we want to be able to undo all previous
    // movements, so let's hold on to those pointers.
    delete piece;
    piece = newPiece;
}

Piece* Square::removePiece()
{
    Piece* _piece = piece;
    piece = 0;

    return _piece;
}

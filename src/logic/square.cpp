// system includes

// class dependencies
#include "square.h"

Square::Square(Piece* piece) : piece(piece)
{

}


const std::string Square::getName() const
{
    if (piece)
    {
        return piece->getName();
    }

    return "empty";
}

enum Colours Square::getColour() const
{
    if (piece)
    {
        return piece->getColour();
    }

    return UNDEFINED;
}

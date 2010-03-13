// system includes

// class dependencies
#include "square.h"

Square::Square() : piece(0), holdsUnit(false)
{

}

Square::Square(Piece* piece) : piece(piece), holdsUnit(true)
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

const std::string Square::getColour() const
{
    if (piece)
    {
        return piece->getColour();
    }

    return "empty";
}

#ifndef _NALCG_SQUARE_H_
#define _NALCG_SQUARE_H_

// system includes
#include <string>

// class includes
#include "piece.h"

class Square
{
    // Constants

    // Members
    Piece* piece;

    // Methods

public:
    Square(Piece* piece = 0);
    virtual ~Square() { delete piece; }

    // Methods
    bool hasPiece() { return piece != 0; }
    void addPiece(Piece* newPiece) { piece = newPiece; }

    // Getters
    const std::string getNameOfPiece() const;
    Piece::Colour getColourOfPiece() const;
    
    // Setters
};

#endif // _NALCG_SQUARE_H_

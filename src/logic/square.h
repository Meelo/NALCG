#ifndef _NALCG_SQUARE_H_
#define _NALCG_SQUARE_H_

// class includes
#include "colour.h"

// system includes
#include <string>

class Piece;
class Square
{
    // Constants

    // Members
    Piece* piece;

    // Methods

public:
    Square(Piece* piece = 0);
    Square(const Square& orig);
    virtual ~Square() { }

    // Methods
    bool hasPiece() const { return piece != 0; }
    void addPiece(Piece* newPiece);
    Piece* removePiece();
    Piece* getPiece() const { return piece; }

    // Getters
    const std::string getNameOfPiece() const;
    Colour getColourOfPiece() const;
    char getSymbolOfPiece(bool markSpecialSymbols = false) const;

    // Setters

};

#endif // _NALCG_SQUARE_H_

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
    bool holdsUnit;

    // Methods

public:
    Square();
    Square(Piece* piece);
    virtual ~Square() { }

    // Methods
    bool hasUnit() { return holdsUnit; }

    // Getters
    const std::string getName() const;
    const std::string getColour() const;
    
    // Setters
};

#endif // _NALCG_SQUARE_H_

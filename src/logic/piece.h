#ifndef _NALCG_PIECE_H_
#define _NALCG_PIECE_H_

// system includes
#include <string>

// class includes
#include "colours.h"

class Piece
{
    // Constants

    // Members
    const enum Colours colour;
    const std::string name;

    // Methods

public:
    Piece(const enum Colours& colour, const std::string& name);
    virtual ~Piece() { }

    // Methods

    // Getters
    const enum Colours& getColour() const { return colour; }
    const std::string& getName() const { return name; };
    
    // Setters
};

#endif // _NALCG_PIECE_H_

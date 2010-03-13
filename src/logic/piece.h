#ifndef _NALCG_PIECE_H_
#define _NALCG_PIECE_H_

// system includes
#include <string>

// class includes

class Piece
{
    // Constants

    // Members
    const std::string colour;
    const std::string name;

    // Methods

public:
    Piece(const std::string& colour, const std::string& name);
    virtual ~Piece() { }

    // Methods

    // Getters
    const std::string getName() const { return name; };
    const std::string getColour() const { return colour; }
    
    // Setters
};

#endif // _NALCG_PIECE_H_

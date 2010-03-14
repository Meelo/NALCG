#ifndef _NALCG_PIECE_H_
#define _NALCG_PIECE_H_

// system includes
#include <string>

// class includes

class Piece
{
public:
    enum Colour
    {
        UNDEFINED,
        WHITE,
        BLACK
    };

private:
    // Constants

    // Members
    const Colour colour;
    const std::string name;

    // Methods

public:
    Piece(const Colour& colour, const std::string& name);
    virtual ~Piece() { }

    // Methods

    // Getters
    const Colour& getColour() const { return colour; }
    const std::string& getName() const { return name; };
    
    // Setters
};

#endif // _NALCG_PIECE_H_

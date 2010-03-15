#ifndef _NALCG_PIECE_H_
#define _NALCG_PIECE_H_

// system includes
#include <string>
#include <vector>

// class includes

class Square;
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
    virtual std::vector<std::size_t> getValidMoves(std::size_t location, const std::vector<Square>& square);

    // Getters
    const Colour& getColour() const { return colour; }
    const std::string& getName() const { return name; };
    
    // Setters
};

#endif // _NALCG_PIECE_H_

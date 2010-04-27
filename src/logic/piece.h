#ifndef _NALCG_PIECE_H_
#define _NALCG_PIECE_H_

// class includes
#include "square.h"
#include "colour.h"

// system includes
#include <string>
#include <vector>

class Piece
{

protected:
    // Constants

    // Members
    const Colour colour;
    const std::string name;
    const char symbol;

    // Methods
    Colour getOppositeColour() const;
    bool isOppositeColour(const Colour& colour) const;
    bool isEmptyOrEdible(std::size_t index, const std::vector<Square>& squares) const;

public:
    Piece(const Colour& colour, const std::string& name, const char symbol = 0);
    virtual ~Piece() { }
    virtual Piece* clone() const = 0;

    // Methods
    virtual std::vector<std::size_t> getValidMoves(std::size_t location,
        const std::vector<Square>& squares, std::size_t protect = ~0) const = 0;
    virtual void specialMoveBehaviour(std::size_t from, std::size_t to);
    virtual void resetRoundSpecificState() { }
    virtual bool trySpecialMove(std::size_t& fromX, std::size_t& fromY,
                                std::size_t& toX,   std::size_t& toY,
                                std::vector<Square>& squares) const;

    static Colour getOppositeColour(Colour colour);

    // Getters
    const Colour& getColour() const { return colour; }
    const std::string& getName() const { return name; };
    virtual char getSymbol(bool markSpecialSymbols = false) const;
    virtual bool isSpecialMoveAllowed() const { return false; }

    // Setters
};

#endif // _NALCG_PIECE_H_

#ifndef _NALCG_KNIGHT_H_
#define _NALCG_KNIGHT_H_

// class includes
#include "piece.h"
#include "colour.h"
#include "square.h"

// system includes

class Knight : public Piece
{
    // Constants
    static const int X_DIRECTIONS[];
    static const int Y_DIRECTIONS[];

    // Members

    // Methods

public:
    Knight(const Colour& colour);
    virtual ~Knight() { }
    Knight* clone() const { return new Knight(*this); }

    // Methods

    // Getters
    std::vector<std::size_t> getValidMoves(std::size_t location, const std::vector<Square>& squares) const;

    // Setters
};

#endif // _NALCG_KNIGHT_H_

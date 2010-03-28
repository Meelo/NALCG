#ifndef _NALCG_KING_H_
#define _NALCG_KING_H_

// system includes

// class includes
#include "piece.h"

class King : public Piece
{
    // Constants
    static const int X_DIRECTIONS[];
    static const int Y_DIRECTIONS[];

    // Members

    // Methods

public:
    King(const Piece::Colour& colour);
    virtual ~King() { }
    King* clone() const { return new King(*this); }

    // Methods

    // Getters
    std::vector<std::size_t> getValidMoves(std::size_t location, const std::vector<Square>& squares) const;

    // Setters
};

#endif // _NALCG_KING_H_

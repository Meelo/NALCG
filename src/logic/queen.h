#ifndef _NALCG_QUEEN_H_
#define _NALCG_QUEEN_H_

// system includes

// class includes
#include "piece.h"

class Queen : public Piece
{
    // Constants
    static const int X_DIRECTIONS[];
    static const int Y_DIRECTIONS[];

    // Members

    // Methods

public:
    Queen(const Piece::Colour& colour);
    virtual ~Queen() { }
    Queen* clone() const { return new Queen(*this); }

    // Methods

    // Getters
    std::vector<std::size_t> getValidMoves(std::size_t location, const std::vector<Square>& squares) const;

    // Setters
};

#endif // _NALCG_QUEEN_H_

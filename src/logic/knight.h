#ifndef _NALCG_KNIGHT_H_
#define _NALCG_KNIGHT_H_

// system includes

// class includes
#include "piece.h"

class Knight : public Piece
{
    // Constants
    static const int X_DIRECTIONS[];
    static const int Y_DIRECTIONS[];

    // Members

    // Methods

public:
    Knight(const Piece::Colour& colour);
    virtual ~Knight() { }

    // Methods

    // Getters
    std::vector<std::size_t> getValidMoves(std::size_t location, const std::vector<Square>& squares) const;
    
    // Setters
};

#endif // _NALCG_KNIGHT_H_

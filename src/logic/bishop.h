#ifndef _NALCG_BISHOP_H_
#define _NALCG_BISHOP_H_

// system includes

// class includes
#include "piece.h"

class Bishop : public Piece
{
    // Constants

    // Members

    // Methods

public:
    Bishop(const Piece::Colour& colour);
    virtual ~Bishop() { }

    // Methods
    std::vector<std::size_t> getValidMoves(std::size_t location, const std::vector<Square>& square) const;

    // Getters
    
    // Setters
};

#endif // _NALCG_BISHOP_H_

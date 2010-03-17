#ifndef _NALCG_BISHOP_H_
#define _NALCG_BISHOP_H_

// system includes

// class includes
#include "piece.h"

class Bishop : public Piece
{
    // Constants
    const int DIAGONAL_MOVE_UP_LEFT;
    const int DIAGONAL_MOVE_UP_RIGHT;
    const int DIAGONAL_MOVE_DOWN_LEFT;
    const int DIAGONAL_MOVE_DOWN_RIGHT;

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

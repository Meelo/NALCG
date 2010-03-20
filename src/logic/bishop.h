#ifndef _NALCG_BISHOP_H_
#define _NALCG_BISHOP_H_

// system includes

// class includes
#include "piece.h"

class Bishop : public Piece
{
    // Constants
    static const int X_DIRECTIONS[];
    static const int Y_DIRECTIONS[];

    // Members

    // Methods
    bool isEmptyOrEdible(std::size_t index, const std::vector<Square>& squares) const;

public:
    Bishop(const Piece::Colour& colour);
    virtual ~Bishop() { }

    // Methods
    std::vector<std::size_t> getValidMoves(std::size_t location, const std::vector<Square>& square) const;

    // Getters
    
    // Setters
};

#endif // _NALCG_BISHOP_H_

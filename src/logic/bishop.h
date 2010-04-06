#ifndef _NALCG_BISHOP_H_
#define _NALCG_BISHOP_H_

// class includes
#include "piece.h"
#include "colour.h"
#include "square.h"

// system includes

class Bishop : public Piece
{
    // Constants
    static const int X_DIRECTIONS[];
    static const int Y_DIRECTIONS[];

    // Members

    // Methods

public:
    Bishop(const Colour& colour);
    virtual ~Bishop() { }
    Bishop* clone() const { return new Bishop(*this); }

    // Methods
    virtual std::vector<std::size_t> getValidMoves(std::size_t location,
        const std::vector<Square>& squares, std::size_t protect = ~0) const;

    // Getters

    // Setters
};

#endif // _NALCG_BISHOP_H_

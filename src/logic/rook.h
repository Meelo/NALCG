#ifndef _NALCG_ROOK_H_
#define _NALCG_ROOK_H_

// class includes
#include "piece.h"
#include "colour.h"
#include "square.h"

// system includes

class Rook : public Piece
{
    // Constants
    static const int X_DIRECTIONS[];
    static const int Y_DIRECTIONS[];

    // Members
    bool specialMoveAllowed;

    // Methods

public:
    Rook(const Colour& colour);
    virtual ~Rook() { }
    Rook* clone() const { return new Rook(*this); }

    // Methods
    virtual void specialMoveBehaviour(std::size_t from, std::size_t to);

    // Getters
    virtual std::vector<std::size_t> getValidMoves(std::size_t location,
        const std::vector<Square>& squares, std::size_t protect = ~0) const;
    virtual bool isSpecialMoveAllowed() const { return specialMoveAllowed; }
    // Setters
};

#endif // _NALCG_ROOK_H_

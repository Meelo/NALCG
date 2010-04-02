#ifndef _NALCG_PAWN_H_
#define _NALCG_PAWN_H_

// system includes
#include <vector>

// class includes
#include "board.h"
#include "piece.h"
#include "square.h"

class Pawn : public Piece
{
    // Constants

    // Members
    bool specialMoveAllowed;
    bool resetOnNextTurn;

    // Methods
    bool isEnPassantAllowed(std::size_t diag, std::size_t side,
        const std::vector<Square>& squares) const;

public:
    Pawn(const Piece::Colour& colour);
    virtual ~Pawn() { }
    Pawn* clone() const { return new Pawn(*this); }

    // Methods
    virtual void specialMoveBehaviour(std::size_t from, std::size_t to);
    virtual void resetRoundSpecificState();

    // Getters
    std::vector<std::size_t> getValidMoves(std::size_t location, const std::vector<Square>& squares) const;
    virtual bool isSpecialMoveAllowed() const { return specialMoveAllowed; }

    // Setters
};

#endif // _NALCG_PAWN_H_

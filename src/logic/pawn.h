#ifndef _NALCG_PAWN_H_
#define _NALCG_PAWN_H_

// class includes
#include "piece.h"
#include "colour.h"
#include "square.h"

// system includes
#include <vector>

class Pawn : public Piece
{
    // Constants

    // Members
    bool specialMoveAllowed;
    bool resetOnNextTurn;

    // Methods
    bool isEatingAllowed(std::size_t target, std::size_t side,
        const std::vector<Square>& squares) const;
    bool isEnPassantAllowed(std::size_t diag, std::size_t side,
        const std::vector<Square>& squares) const;
    bool isMoveAllowed(std::size_t location,
        const std::vector<Square>& squares) const;
    bool isDoubleMoveAllowed(std::size_t row) const;

public:
    Pawn(const Colour& colour);
    virtual ~Pawn() { }
    Pawn* clone() const { return new Pawn(*this); }

    // Methods
    virtual void specialMoveBehaviour(std::size_t from, std::size_t to);
    virtual void resetRoundSpecificState();
    virtual bool trySpecialMove(std::size_t& fromX, std::size_t& fromY,
                                std::size_t& toX,   std::size_t& toY,
                                std::vector<Square>& squares) const;

    // Getters
    virtual std::vector<std::size_t> getValidMoves(std::size_t location,
        const std::vector<Square>& squares, std::size_t protect = ~0) const;
    virtual bool isSpecialMoveAllowed() const { return specialMoveAllowed; }

    // Setters
};

#endif // _NALCG_PAWN_H_

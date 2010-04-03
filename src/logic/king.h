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
    static const int CASTLING;
    static const int LEFT_ROOK_X;
    static const int RIGHT_ROOK_X;

    // Members
    bool specialMoveAllowed;


    // Methods
    bool isCastlingAllowed(std::size_t rookLocation,
        const std::vector<Square>& squares) const;

public:
    King(const Piece::Colour& colour);
    virtual ~King() { }
    King* clone() const { return new King(*this); }

    // Methods
    virtual void specialMoveBehaviour(std::size_t from, std::size_t to);

    // Getters
    std::vector<std::size_t> getValidMoves(std::size_t location, const std::vector<Square>& squares) const;
    virtual bool isSpecialMoveAllowed() const { return specialMoveAllowed; }

    // Setters
};

#endif // _NALCG_KING_H_

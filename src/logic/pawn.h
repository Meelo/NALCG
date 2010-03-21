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

    // Methods

public:
    Pawn(const Piece::Colour& colour);
    virtual ~Pawn() { }

    // Methods
    std::vector<std::size_t> getValidMoves(std::size_t location, const std::vector<Square>& square) const;

    // Getters
    
    // Setters
};

#endif // _NALCG_PAWN_H_

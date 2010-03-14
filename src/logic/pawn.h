#ifndef _NALCG_PAWN_H_
#define _NALCG_PAWN_H_

// system includes

// class includes
#include "piece.h"
#include "colours.h"

class Pawn : public Piece
{
    // Constants

    // Members

    // Methods

public:
    Pawn(const enum Colours& colour);
    virtual ~Pawn() { }

    // Methods

    // Getters
    
    // Setters
};

#endif // _NALCG_PAWN_H_

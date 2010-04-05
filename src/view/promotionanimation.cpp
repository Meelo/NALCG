#include "promotionanimation.h"

#include "view.h"
#include "../logic/chessboard.h"

bool PromotionAnimation::animate(const Real& timeSinceLastFrame)
{
    char upperCaseSymbol = promotionToSymbol(mPromoteTo);
    char symbol = upperCaseSymbol;
    if (!mView->isWhiteTurn())
    {
        symbol |= 1 << 5;
    }
    mView->createPiece(upperCaseSymbol, mView->getMeshName(symbol), mAnimatedNode->getPosition());

    mSceneMgr->getRootSceneNode()->removeAndDestroyChild(mAnimatedNode->getName());
    return false;
}

char PromotionAnimation::promotionToSymbol(unsigned int promoteTo) const
{
    if (promoteTo == ChessBoard::PROMOTE_TO_QUEEN) return 'Q';
    if (promoteTo == ChessBoard::PROMOTE_TO_KNIGHT) return 'N';
    if (promoteTo == ChessBoard::PROMOTE_TO_ROOK) return 'R';
    if (promoteTo == ChessBoard::PROMOTE_TO_BISHOP) return 'B';
    return 0;
}
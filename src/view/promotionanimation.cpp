#include "promotionanimation.h"

#include "view.h"
#include "../logic/chessboard.h"

bool PromotionAnimation::animate(const Real& timeSinceLastFrame)
{
    if (mPromotionDuration >= 0 || !transformed)
    {
        if (!mParticleNode && !transformed)
        {
            createBlasts();
        }
        
        Vector3 position = mAnimatedNode->getPosition();
        position.y = sin((mPromotionDuration - 1.5) * Math::PI / (PROMOTION_DURATION - 1.5)) * 150;
        mAnimatedNode->setPosition(position);
        mAnimatedNode->yaw(Degree(timeSinceLastFrame * 360 / (PROMOTION_DURATION - 1.5)));

        if (!mWhiteFlashNode && mPromotionDuration < 3)
        {
            createWhiteFlash();
        }
        if (!transformed && mPromotionDuration < 1.5)
        {
            char upperCaseSymbol = promotionToSymbol(mPromoteTo);
            char symbol = upperCaseSymbol;
            if (!mView->isWhiteTurn())
            {
                symbol |= 1 << 5;
            }
            position.y = 0;
            mView->createPiece(upperCaseSymbol, mView->getMeshName(symbol), position);

            mAnimatedNode->scale(0.5, 0.5, 0.5);

            mAnimatedNode->removeAndDestroyChild(mParticleNode->getName());
            mParticleNode = 0;

            transformed = true;
        }


        mPromotionDuration -= timeSinceLastFrame;
        return true;
    }

    mAnimatedNode->removeAndDestroyChild(mWhiteFlashNode->getName());
    mWhiteFlashNode = 0;

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

void PromotionAnimation::createBlasts()
{
    ParticleSystem* pSys = mSceneMgr->createParticleSystem(
        nextName(), "Effects/Aureola");
    mParticleNode = mAnimatedNode->createChildSceneNode();
    //const Vector3& position = pieceNode->getPosition();
    mParticleNode->translate(0, 0, 0);
    mParticleNode->attachObject(pSys);
    ParticleEmitter *emitter = pSys->getEmitter(0);

    pSys->fastForward(0.5);
    //pSys->setParticleQuota(emitter->getEmissionRate() * mPromotionDuration);
}


void PromotionAnimation::createWhiteFlash()
{
    ParticleSystem* pSys = mSceneMgr->createParticleSystem(
        nextName(), "Effects/WhiteFlash");
    mWhiteFlashNode = mAnimatedNode->createChildSceneNode();
    //const Vector3& position = pieceNode->getPosition();
    mWhiteFlashNode->translate(0, 150, 0);
    mWhiteFlashNode->attachObject(pSys);
    ParticleEmitter *emitter = pSys->getEmitter(0);

    //pSys->setParticleQuota(emitter->getEmissionRate());
}
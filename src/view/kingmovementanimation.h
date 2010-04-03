#ifndef _NALCG_KING_MOVEMENT_ANIMATION_H_
#define _NALCG_KING_MOVEMENT_ANIMATION_H_

#include "movementanimation.h"

class KingMovementAnimation : public MovementAnimation
{
public:
    KingMovementAnimation(const Vector3& destination, SceneNode *movingNode,
        SceneNode *targetPiece, SceneManager *sceneMgr, AnimationManager *animationManager)
        : MovementAnimation(destination, movingNode, targetPiece, sceneMgr, animationManager),
        mAttackDuration(targetPiece ? 3.0 : -1), mParticleNode(0),
        mPhase(1), mHalfTotalDistance((mDestination - mAnimatedNode->getPosition()).length() / 2),
        mFlyingAltitude(2400.0 / mHalfTotalDistance - 9)
    {
    }

    virtual bool animate(const Real& timeSinceLastFrame);
    virtual void createBlasts();
protected:
    static const int MOVEMENT_SPEED = 200;
    Real mAttackDuration;
    SceneNode *mParticleNode;
    int mPhase;
    const Real mHalfTotalDistance;
    const int mFlyingAltitude;
};

#endif // _NALCG_KING_MOVEMENT_ANIMATION_H_

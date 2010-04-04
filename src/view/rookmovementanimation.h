#ifndef _NALCG_ROOK_MOVEMENT_ANIMATION_H_
#define _NALCG_ROOK_MOVEMENT_ANIMATION_H_

#include "movementanimation.h"

class RookMovementAnimation : public MovementAnimation
{
public:
    RookMovementAnimation(const Vector3& destination, SceneNode *movingNode,
        SceneNode *targetPiece, SceneManager *sceneMgr, AnimationManager *animationManager)
        : MovementAnimation(destination, movingNode, targetPiece, sceneMgr, animationManager),
        mAttackDuration(targetPiece ? 3.0 : -1), mStartedAttacking(false)
    {
    }

    virtual bool animate(const Real& timeSinceLastFrame);
protected:
    static const int MOVEMENT_SPEED = 500;
    Real mAttackDuration;
    bool mStartedAttacking;
};

#endif // _NALCG_ROOK_MOVEMENT_ANIMATION_H_

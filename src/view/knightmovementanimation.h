#pragma once

#include "movementanimation.h"

class KnightMovementAnimation : public MovementAnimation
{
public:
    KnightMovementAnimation(const Vector3& destination, SceneNode *movingNode,
        SceneNode *targetPiece, SceneManager *sceneMgr, AnimationManager *animationManager)
        : MovementAnimation(destination, movingNode, targetPiece, sceneMgr, animationManager),
        mHalfTotalDistance((mDestination - mAnimatedNode->getPosition()).length() / 2)
    {
    }

    virtual bool animate(const Real& timeSinceLastFrame);
protected:
    static const int MOVEMENT_SPEED = 500;
    static const int JUMP_HEIGHT = 10;
    const Real mHalfTotalDistance;
};


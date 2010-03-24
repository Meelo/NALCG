#pragma once

#include "movementanimation.h"

class KnightMovementAnimation : public MovementAnimation
{
public:
    KnightMovementAnimation(const Vector3& destination, SceneNode *movingNode,
        SceneNode *targetPiece, SceneManager *sceneMgr, AnimationManager *animationManager)
        : MovementAnimation(destination, movingNode, targetPiece, sceneMgr, animationManager)
    {
    }

    virtual bool animate(const Real& timeSinceLastFrame);
protected:
    static const int MOVEMENT_SPEED = 500;
};


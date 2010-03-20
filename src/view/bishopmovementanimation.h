#pragma once

#include "movementanimation.h"

class BishopMovementAnimation : public MovementAnimation
{
public:
    BishopMovementAnimation(const Vector3& destination, SceneNode *movingNode,
        SceneNode *targetPiece, SceneManager *sceneMgr, AnimationManager *animationManager)
        : MovementAnimation(destination, movingNode, targetPiece, sceneMgr, animationManager)
    {
    }

    virtual bool animate(const Real& timeSinceLastFrame);
protected:
    static const int MOVEMENT_SPEED = 500;
};


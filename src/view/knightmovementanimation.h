#ifndef _NALCG_KNIGHT_MOVEMENT_ANIMATION_H_
#define _NALCG_KNIGHT_MOVEMENT_ANIMATION_H_

#include "movementanimation.h"

class KnightMovementAnimation : public MovementAnimation
{
public:
    KnightMovementAnimation(const Vector3& destination, SceneNode *movingNode,
        SceneNode *targetPiece, SceneManager *sceneMgr, AnimationManager *animationManager)
        : MovementAnimation(destination, movingNode, targetPiece, sceneMgr, animationManager),
        mHalfTotalDistance((mDestination - mAnimatedNode->getPosition()).length() / 2),
        mCrushedEnemy(false), mEndDelay(END_DELAY), mCameraShaken(false)
    {
    }

    virtual bool animate(const Real& timeSinceLastFrame);
protected:
    static const int MOVEMENT_SPEED = 500;
    static const int JUMP_HEIGHT = 10;
    static const Real END_DELAY;
    const Real mHalfTotalDistance;
    bool mCrushedEnemy;
    double mEndDelay;
    bool mCameraShaken;
};

#endif // _NALCG_KNIGHT_MOVEMENT_ANIMATION_H_

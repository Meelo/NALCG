#pragma once

#include <Ogre.h>

#include "movementanimation.h"

using namespace Ogre;

class QueenMovementAnimation : public MovementAnimation
{
public:
    QueenMovementAnimation(const Vector3& destination, SceneNode *movingNode,
        SceneNode *targetPiece, SceneManager *sceneMgr, AnimationManager *animationManager)
        : MovementAnimation(destination, movingNode, targetPiece, sceneMgr, animationManager),
        mAttackCount(targetPiece ? ATTACK_COUNT : 0), mPhase(1), mTrail(0),
        mAttackCooldown(targetPiece ? 0 : -ATTACK_ANIMATION_LENGTH), mBloodCountdown(0),
        mFlyingAltitude(targetPiece ? ATTACK_ALTITUDE : MOVE_ALTITUDE)
    {
        createBlasts();
    }

    virtual ~QueenMovementAnimation()
    {
        if (mPhase < 4)
        {
            destroyAnimationsBillboardsAndTrails();
            mPhase = 4;
        }
        mAnimatedNode->removeAndDestroyAllChildren();

        for (std::size_t i = 0; i < mLights.size(); i++)
        {
            mSceneMgr->destroyLight(mLights.at(i)->getName());
        }

        restoreLights();
    }

    virtual bool animate(const Real& timeSinceLastFrame);
    virtual void createBlasts();
    virtual void destroyAnimationsBillboardsAndTrails();
    virtual void dimLights();
    virtual void restoreLights();

protected:
    static const int MOVEMENT_SPEED;
    static const int ATTACK_ALTITUDE;
    static const int MOVE_ALTITUDE;
    static const int ATTACK_COUNT;
    static const int ATTACK_ANIMATION_LENGTH;
    static const int FLAT_ATTACKING_DISTANCE;
    static const Real ATTACK_COOLDOWN;

    int mAttackCount;
    int mPhase;
    std::vector<AnimationState*> mAnimStateList;
    RibbonTrail* mTrail;
    Real mAttackCooldown;
    std::vector<Light*> mLights;
    Real mBloodCountdown;
    const int mFlyingAltitude;
};



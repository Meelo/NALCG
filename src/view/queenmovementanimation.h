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
        mAttackCooldown(targetPiece ? 0 : -ATTACK_ANIMATION_LENGTH), mBloodCountdown(0)
    {
        createBlasts();
    }

    virtual ~QueenMovementAnimation()
    {
        mAnimatedNode->removeAndDestroyAllChildren();

        for (std::size_t i = 0; i < mLights.size(); i++)
        {
            mSceneMgr->destroyLight(mLights.at(i)->getName());
        }

        restoreLights();
    }

    virtual bool animate(const Real& timeSinceLastFrame);
    virtual void createBlasts();
    virtual void dimLights();
    virtual void restoreLights();

protected:
    static const int MOVEMENT_SPEED = 500;
    static const int FLYING_ALTITUDE = 500;
    static const int ATTACK_COUNT = 30;
    static const int ATTACK_ANIMATION_LENGTH = 3;
    static const int FLAT_ATTACKING_DISTANCE = 150;
    static const Real ATTACK_COOLDOWN;

    int mAttackCount;
    int mPhase;
    std::vector<AnimationState*> mAnimStateList;
    RibbonTrail* mTrail;
    Real mAttackCooldown;
    std::vector<Light*> mLights;
    Real mBloodCountdown;
};



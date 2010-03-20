#pragma once

#include "genericanimation.h"

class BleedingAnimation : public GenericAnimation
{
public:
    BleedingAnimation(SceneNode *particleNode, SceneManager *sceneMgr, const Real& duration)
        : GenericAnimation(particleNode, sceneMgr), mDuration(duration)
    {
    }

    ~BleedingAnimation()
    {
        mSceneMgr->getRootSceneNode()->removeAndDestroyChild(mAnimatedNode->getName());
    }

    virtual bool animate(const Real& timeSinceLastFrame)
    {
        mDuration -= timeSinceLastFrame;
        return mDuration >= 0;
    }

protected:
    Real mDuration;
};


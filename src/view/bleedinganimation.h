#pragma once

#include "genericanimation.h"

class BleedingAnimation : public GenericAnimation
{
public:
    BleedingAnimation(SceneNode *pieceNode, SceneManager *sceneMgr,
        const Real& duration, SceneNode *particleNode, const Real& delay)
        : GenericAnimation(pieceNode, sceneMgr), mDuration(duration),
        mParticleNode(particleNode), mAnimatedNodeName(pieceNode->getName()),
        mDelay(delay)
    {
    }

    ~BleedingAnimation()
    {
        if (mSceneMgr->hasSceneNode(mAnimatedNodeName))
        {
            mAnimatedNode->removeAndDestroyChild(mParticleNode->getName());
        }
    }

    virtual bool animate(const Real& timeSinceLastFrame)
    {
        mDelay -= timeSinceLastFrame;
        if (mDelay <= 0)
        {
            mDuration -= timeSinceLastFrame;
        }
        return mDuration >= 0;
    }

protected:
    double mDelay;
    Real mDuration;
    SceneNode *mParticleNode;
    std::string mAnimatedNodeName;
};


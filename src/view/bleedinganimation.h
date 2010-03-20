#pragma once

#include "genericanimation.h"

class BleedingAnimation : public GenericAnimation
{
public:
    BleedingAnimation(SceneNode *pieceNode, SceneManager *sceneMgr,
        const Real& duration, SceneNode *particleNode)
        : GenericAnimation(pieceNode, sceneMgr), mDuration(duration),
        mParticleNode(particleNode), mAnimatedNodeName(pieceNode->getName())
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
        mDuration -= timeSinceLastFrame;
        return mDuration >= 0;
    }

protected:
    Real mDuration;
    SceneNode *mParticleNode;
    std::string mAnimatedNodeName;
};


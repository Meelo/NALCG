#ifndef _NALCG_BLEEDING_ANIMATION_H_
#define _NALCG_BLEEDING_ANIMATION_H_

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
            SceneNode::ObjectIterator itr = mParticleNode->getAttachedObjectIterator();
            while (itr.hasMoreElements())
            {
                MovableObject* object = itr.getNext();
                mSceneMgr->destroyMovableObject(object);
            }
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
    Real mDuration;
    SceneNode *mParticleNode;
    std::string mAnimatedNodeName;
    double mDelay;
};

#endif // _NALCG_BLEEDING_ANIMATION_H_

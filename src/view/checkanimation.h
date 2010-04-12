#ifndef _NALCG_CHECK_ANIMATION_H_
#define _NALCG_CHECK_ANIMATION_H_

#include "genericanimation.h"

class CheckAnimation : public GenericAnimation
{
public:
    CheckAnimation(SceneNode *checkedNode, SceneManager *sceneMgr)
        : GenericAnimation(checkedNode, sceneMgr), mTextNode(0),
        mDuration(3)
    {
    }

    virtual ~CheckAnimation()
    {
        SceneNode::ObjectIterator itr = mTextNode->getAttachedObjectIterator();
        while (itr.hasMoreElements())
        {
            MovableObject* object = itr.getNext();
            mSceneMgr->destroyMovableObject(object);
        }

        mAnimatedNode->removeAndDestroyChild(mTextNode->getName());
        mTextNode = 0;
    }

    virtual bool animate(const Real& timeSinceLastFrame)
    {
        if (!mTextNode)
        {
            Entity* ent = mSceneMgr->createEntity(nextName(), "check.mesh");
            ent->setQueryFlags(0);
            mTextNode = mAnimatedNode->createChildSceneNode(Vector3(0, 1000, 0));
            mTextNode->attachObject(ent);
        }
        if (mDuration < 0.5)
        {
            mTextNode->translate(0, timeSinceLastFrame * 2000, 0);
        }
        else if (mTextNode->getPosition().y >= 0)
        {
            mTextNode->translate(0, -timeSinceLastFrame * 2000, 0);
        }
        else
        {
            mTextNode->yaw(Radian(timeSinceLastFrame * Math::PI));
        }
        //mTextNode->translate(0, timeSinceLastFrame * 500, 0);
        mDuration -= timeSinceLastFrame;

        return mDuration >= 0;
    }

protected:
    SceneNode* mTextNode;
    double mDuration;

};

#endif // _NALCG_CHECK_ANIMATION_H_

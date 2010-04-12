#ifndef _NALCG_CHECK_ANIMATION_H_
#define _NALCG_CHECK_ANIMATION_H_

#include "genericanimation.h"

class CheckAnimation : public GenericAnimation
{
public:
    CheckAnimation(SceneNode *checkedNode, SceneManager *sceneMgr,
        double delay)
        : GenericAnimation(checkedNode, sceneMgr), mDelay(delay), mTextNode(0),
        mAngle(0)
    {
    }

    virtual ~CheckAnimation()
    {
    }

    virtual bool animate(const Real& timeSinceLastFrame)
    {
        mDelay -= timeSinceLastFrame;
        if (mDelay <= 0)
        {
            if (!mTextNode)
            {
                Entity* ent = mSceneMgr->createEntity(nextName(), "check.mesh");
                ent->setQueryFlags(0);
                mTextNode = mSceneMgr->createSceneNode();
                mTextNode->attachObject(ent);
            }
            mTextNode->yaw(Radian(timeSinceLastFrame));
            mTextNode->translate(0, timeSinceLastFrame * 500, 0);
            mAngle += timeSinceLastFrame;

            return mAngle < Math::PI * 2;
        }
        return true;
    }

protected:
    double mDelay;
    SceneNode* mTextNode;
    Real mAngle;

};

#endif // _NALCG_CHECK_ANIMATION_H_

#pragma once

#include "genericanimation.h"

class DyingAnimation : public GenericAnimation
{
public:
    DyingAnimation(SceneNode *dyingNode, SceneManager *sceneMgr,
        double delay)
        : GenericAnimation(dyingNode, sceneMgr), mDelay(delay), mAngle(Math::PI / 2)
    {
    }

    virtual ~DyingAnimation()
    {
    }

    virtual bool animate(const Real& timeSinceLastFrame)
    {
        mDelay -= timeSinceLastFrame;
        if (mDelay <= 0 && mAngle >= 0)
        {
            mAnimatedNode->pitch(-Radian(timeSinceLastFrame));
            mAngle -= timeSinceLastFrame;
        }
        return true;
    }

protected:
    double mDelay;
    double mAngle;

};

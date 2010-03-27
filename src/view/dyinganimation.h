#ifndef _NALCG_DYING_ANIMATION_H_
#define _NALCG_DYING_ANIMATION_H_

#include "genericanimation.h"

class DyingAnimation : public GenericAnimation
{
public:
    DyingAnimation(SceneNode *dyingNode, SceneManager *sceneMgr,
        double delay, double dyingSpeedMultiplier)
        : GenericAnimation(dyingNode, sceneMgr), mDelay(delay),
        mAngle(Math::PI / 2), mDyingSpeedMultiplier(dyingSpeedMultiplier)
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
            double angleChange = timeSinceLastFrame * mDyingSpeedMultiplier;
            mAnimatedNode->pitch(-Radian(angleChange));
            mAngle -= angleChange;
        }
        return true;
    }

protected:
    double mDelay;
    double mAngle;
    double mDyingSpeedMultiplier;

};

#endif // _NALCG_DYING_ANIMATION_H_

#pragma once

#include "genericanimation.h"

class CameraShakeAnimation : public GenericAnimation
{
public:
    CameraShakeAnimation(Camera* camera, SceneManager *sceneMgr)
        : GenericAnimation(0, sceneMgr), mCamera(camera),
        mShakeDuration(SHAKE_DURATION), mInitialOrientation(camera->getOrientation())
    {
    }

    virtual ~CameraShakeAnimation()
    {
    }

    virtual bool animate(const Real& timeSinceLastFrame)
    {
        if (mShakeDuration >= 0)
        {
            int direction;
            if (int(mShakeDuration * SHAKE_FREQUENCY) % 2 == 0)
            {
                direction = COUNTER_CLOCKWISE;
            }
            else
            {
                direction = CLOCKWISE;
            }

            mCamera->roll(Radian(timeSinceLastFrame * direction));
            mShakeDuration -= timeSinceLastFrame;
            return true;
        }
        mCamera->setOrientation(mInitialOrientation);
        return false;
    }

protected:
    static const Real SHAKE_DURATION;
    static const int CLOCKWISE = -1;
    static const int COUNTER_CLOCKWISE = 1;
    static const int SHAKE_FREQUENCY = 10;
    Camera* mCamera;
    double mShakeDuration;
    Quaternion mInitialOrientation;

};

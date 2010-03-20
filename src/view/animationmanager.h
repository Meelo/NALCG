#pragma once

#include <vector>

class GenericAnimation;
class AnimationManager
{
public:
    virtual void addAnimation(GenericAnimation *animation)
    {
        mGenericAnimations.push_back(animation);
    }

    virtual void executeAnimations(double timeSinceLastFrame);
protected:
    std::vector<GenericAnimation*> mGenericAnimations;
};


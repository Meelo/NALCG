#pragma once

#include <vector>

#include <Ogre.h>

using namespace Ogre;

class GenericAnimation;
class AnimationManager
{
public:
    virtual void addAnimation(GenericAnimation *animation)
    {
        mGenericAnimations.push_back(animation);
    }

    virtual void executeAnimations(double timeSinceLastFrame);
    virtual void stopAllAnimationsBelongingTo(SceneNode *targetNode);

protected:
    std::vector<GenericAnimation*> mGenericAnimations;

    virtual void endAnimation(std::size_t index);
};


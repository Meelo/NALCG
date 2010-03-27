#ifndef _NALCG_ANIMATION_MANAGER_H_
#define _NALCG_ANIMATION_MANAGER_H_

#include <vector>

#include <Ogre.h>

using namespace Ogre;

class GenericAnimation;
class AnimationManager
{
public:
    AnimationManager() : mAnimationSpeedMultiplier(1.0)
    {
    }

    virtual void addAnimation(GenericAnimation *animation)
    {
        mGenericAnimations.push_back(animation);
    }

    virtual void setAnimationSpeedMultiplier(float animationSpeedMultiplier)
    {
        mAnimationSpeedMultiplier = animationSpeedMultiplier;
    }

    virtual void executeAnimations(double timeSinceLastFrame);
    virtual void stopAllAnimationsBelongingTo(SceneNode *targetNode);

protected:
    std::vector<GenericAnimation*> mGenericAnimations;
    float mAnimationSpeedMultiplier;
    virtual void endAnimation(std::size_t index);
};

#endif // _NALCG_ANIMATION_MANAGER_H_

#include "animationmanager.h"
#include "genericanimation.h"

void AnimationManager::executeAnimations(double timeSinceLastFrame)
{
    for (std::size_t i = mGenericAnimations.size() - 1; i != -1; i--)
    {
        if (!mGenericAnimations.at(i)->animate(timeSinceLastFrame))
        {
            endAnimation(i);
        }
    }
}

void AnimationManager::stopAllAnimationsBelongingTo(SceneNode *targetNode)
{
    for (std::size_t i = mGenericAnimations.size() - 1; i != -1; i--)
    {
        if (mGenericAnimations.at(i)->getAnimatedNode() == targetNode)
        {
            endAnimation(i);
        }
    }
}

void AnimationManager::endAnimation(std::size_t index)
{
    std::swap(mGenericAnimations.at(index), mGenericAnimations.back());
    delete mGenericAnimations.back();
    mGenericAnimations.pop_back();
}
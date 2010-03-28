#include "animationmanager.h"
#include "genericanimation.h"

void AnimationManager::executeAnimations(double timeSinceLastFrame, bool instant)
{
    double timeAdvancement = timeSinceLastFrame * mAnimationSpeedMultiplier;
    if (instant)
    {
        timeAdvancement = std::numeric_limits<float>::max();
    }
    std::size_t max = std::numeric_limits<std::size_t>::max();
    for (std::size_t i = mGenericAnimations.size() - 1; i != max; i--)
    {
        if (!mGenericAnimations.at(i)->animate(timeAdvancement))
        {
            endAnimation(i);
        }

        // In case ending one animation caused other animations to end.
        if (i > mGenericAnimations.size())
        {
            i = mGenericAnimations.size();
        }
    }
}

void AnimationManager::finishAnimations()
{
    while (animationsRunning())
    {
        executeAnimations(0, true);
    }
}

void AnimationManager::stopAllAnimationsBelongingTo(SceneNode *targetNode)
{
    std::size_t max = std::numeric_limits<std::size_t>::max();
    for (std::size_t i = mGenericAnimations.size() - 1; i != max; i--)
    {
        if (mGenericAnimations.at(i)->getAnimatedNode() == targetNode)
        {
            endAnimation(i);
        }
        
        // In case ending one animation caused other animations to end.
        if (i > mGenericAnimations.size())
        {
            i = mGenericAnimations.size();
        }
    }
}

void AnimationManager::endAnimation(std::size_t index)
{
    std::swap(mGenericAnimations.at(index), mGenericAnimations.back());
    GenericAnimation* animation = mGenericAnimations.back();
    mGenericAnimations.pop_back();
    // First pop it, then delete it in case the deletion causes another deletion.
    delete animation;

}
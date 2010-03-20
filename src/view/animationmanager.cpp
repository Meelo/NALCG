#include "animationmanager.h"
#include "genericanimation.h"

void AnimationManager::executeAnimations(double timeSinceLastFrame)
{
    for (std::size_t i = mGenericAnimations.size() - 1; i != -1; i--)
    {
        GenericAnimation *animation = mGenericAnimations.at(i);
        if (!animation->animate(timeSinceLastFrame))
        {
            std::swap(mGenericAnimations.at(i), mGenericAnimations.back());
            delete animation;
            mGenericAnimations.pop_back();
        }
    }
}

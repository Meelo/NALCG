#ifndef _NALCG_ANIMATION_MANAGER_H_
#define _NALCG_ANIMATION_MANAGER_H_

#include <Ogre.h>

using namespace Ogre;

class View;
class GenericAnimation;
class AnimationManager
{
public:
    AnimationManager(View* view) : mAnimationSpeedMultiplier(1.0), mView(view)
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

    virtual void executeAnimations(double timeSinceLastFrame, bool instant = false);
    virtual void finishAnimations();
    virtual void stopAllAnimationsBelongingTo(SceneNode *targetNode);
    virtual View* getView() const { return mView; }
    virtual bool animationsRunning() const
    {
        return !mGenericAnimations.empty();
    }

protected:
    std::vector<GenericAnimation*> mGenericAnimations;
    View* mView;

    float mAnimationSpeedMultiplier;
    virtual void endAnimation(std::size_t index);
};

#endif // _NALCG_ANIMATION_MANAGER_H_

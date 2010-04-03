#ifndef _NALCG_MOVEMENT_ANIMATION_H_
#define _NALCG_MOVEMENT_ANIMATION_H_

#include "genericanimation.h"
#include "animationmanager.h"

class MovementAnimation : public GenericAnimation
{
public:
    MovementAnimation(const Vector3& destination, SceneNode *movingNode,
        SceneNode *targetPiece, SceneManager *sceneMgr, AnimationManager *animationManager)
        : GenericAnimation(movingNode, sceneMgr), mDestination(destination),
        mTargetPiece(targetPiece), mTargetPieceName(targetPiece ? targetPiece->getName() : "NO_TARGET"),
        mAnimationManager(animationManager)
    {
    }

    virtual ~MovementAnimation()
    {
        // Make sure another shorter animation hasn't already destroyed the piece.
        // Using target piece name since target piece pointer might already be freed
        // and isn't safe to call.
        if (mSceneMgr->hasSceneNode(mTargetPieceName))
        {
            mAnimationManager->stopAllAnimationsBelongingTo(mTargetPiece);
            mSceneMgr->getRootSceneNode()->removeAndDestroyChild(mTargetPieceName);
        }
    }
    virtual void dimLights();
    virtual void restoreLights();

protected:
    virtual void playAnimation(const std::string& animationName, double time);

    Vector3 mDestination;
    SceneNode *mTargetPiece;
    const std::string mTargetPieceName;
    AnimationManager *mAnimationManager;
};

#endif // _NALCG_MOVEMENT_ANIMATION_H_

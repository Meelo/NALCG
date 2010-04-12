#include "rookmovementanimation.h"

#include "animationfactory.h"
#include "dyinganimation.h"
#include "bleedinganimation.h"
#include "animationmanager.h"

bool RookMovementAnimation::animate(const Real& timeSinceLastFrame)
{
    Real distanceMoved = MOVEMENT_SPEED * timeSinceLastFrame;
    Vector3 path = mDestination - mAnimatedNode->getPosition();

    if (path.length() > distanceMoved)
    {
        if (path.length() < 180 && mAttackDuration > 0)
        {
            if (mAttackDuration >= 2.0)
            {
                playAnimation("attack", timeSinceLastFrame);
            }
            if (mAttackDuration <= 2.3)
            {
                Vector3 toTarget = mTargetPiece->getPosition() - mAnimatedNode->getPosition();
                toTarget.normalise();

                toTarget.y = (mAttackDuration - 1.3);
                Real speed = timeSinceLastFrame * 1200;
                mTargetPiece->translate(toTarget * speed);
            }
            if (!mStartedAttacking)
            {
                mAnimationManager->addAnimation(
                    AnimationFactory::createDyingAnimation(
                    mTargetPiece, mSceneMgr, 0.8, 1));
                mAnimationManager->addAnimation(
                    AnimationFactory::createBleedingAnimation(
                    mTargetPiece, mSceneMgr, 0.75, 2));
                mStartedAttacking = true;
            }
            mAttackDuration -= timeSinceLastFrame;
        }
        else {
            // Normalising the vector so the speed remains constant.
            path.normalise();
            mAnimatedNode->translate(path * distanceMoved);

            Vector3 src = mAnimatedNode->getOrientation() * Vector3::UNIT_Z;
            mAnimatedNode->rotate(src.getRotationTo(path));

            playAnimation("walk", timeSinceLastFrame);
        }
        return true; // Animation still running.
    }
    //resetAnimation("walk");
    resetAnimation("attack");
    mAnimatedNode->setPosition(mDestination);
    mAnimatedNode->setOrientation(mAnimatedNode->getInitialOrientation());
    return false; // Animation finished.
}

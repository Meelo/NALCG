#include "pawnmovementanimation.h"

#include "animationfactory.h"
#include "dyinganimation.h"
#include "bleedinganimation.h"
#include "animationmanager.h"

bool PawnMovementAnimation::animate(const Real& timeSinceLastFrame)
{
    Real distanceMoved = MOVEMENT_SPEED * timeSinceLastFrame;
    Vector3 path = mDestination - mAnimatedNode->getPosition();

    if (path.length() > distanceMoved)
    {
        if (path.length() < 280 && mAttackDuration > 0)
        {
            if (!mParticleNode)
            {
                createBlasts();
                mAnimationManager->addAnimation(
                    AnimationFactory::createDyingAnimation(
                    mTargetPiece, mSceneMgr, 2, 1.5));
                mAnimationManager->addAnimation(
                    AnimationFactory::createBleedingAnimation(
                    mTargetPiece, mSceneMgr, 1, 1.5));
            }

            if (mParticleNode->getPosition().y + mTargetPiece->getPosition().y >= 0)
            {
                
                if (mParticleNode->getPosition().y < 200)
                {
                    mTargetPiece->translate(0, -timeSinceLastFrame * 600, 0);
                    mParticleNode->translate(0, -timeSinceLastFrame * 400, 0);
                }
                else
                {
                    mParticleNode->translate(0, -timeSinceLastFrame * 1000, 0);
                }
            }
            else
            {
                playAnimation("eat", timeSinceLastFrame, false, mParticleNode, false);
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
    resetAnimation("walk");
    mAnimatedNode->setPosition(mDestination);
    mAnimatedNode->setOrientation(mAnimatedNode->getInitialOrientation());
    return false; // Animation finished.
}

void PawnMovementAnimation::createBlasts()
{
    mParticleNode = mTargetPiece->createChildSceneNode();
    mParticleNode->translate(0, 1000, 0);

    Entity* yellowBall = mSceneMgr->createEntity(nextName(), "yellow_ball.mesh");
    yellowBall->setQueryFlags(0);
    mParticleNode->attachObject(yellowBall);
}
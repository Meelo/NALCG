#include "knightmovementanimation.h"

#include "animationfactory.h"
#include "dyinganimation.h"
#include "camerashakeanimation.h"
#include "bleedinganimation.h"

const Real KnightMovementAnimation::END_DELAY = 0.7;

bool KnightMovementAnimation::animate(const Real& timeSinceLastFrame)
{
    Real distanceMoved = MOVEMENT_SPEED * timeSinceLastFrame;
    Vector3 path = mDestination - mAnimatedNode->getPosition();
    path.y = 0;

    if (path.length() > distanceMoved)
    {
        if (mTargetPiece && path.length() < 50 && !mCrushedEnemy)
        {
            mAnimationManager->addAnimation(
                AnimationFactory::createDyingAnimation(
                mTargetPiece, mSceneMgr, 0, 10));
            mAnimationManager->addAnimation(
                AnimationFactory::createBleedingAnimation(
                mTargetPiece, mSceneMgr, 0, 0.5));
            mCrushedEnemy = true;
        }

        // Initial vertical velocity is half of the length of the total distance
        // Path shortens while knight travels towards its target and has the same
        // effect as gravity would have to the vertical speed.
        Real verticalVelocity = (path.length() - mHalfTotalDistance) * JUMP_HEIGHT;
        mAnimatedNode->translate(Vector3(0, verticalVelocity * timeSinceLastFrame, 0)); 

        Real flipAngle = path.length() * 180 / mHalfTotalDistance;

        // Normalising the vector so the speed remains constant.
        path.normalise();
        mAnimatedNode->translate(path * distanceMoved);

        mAnimatedNode->resetOrientation();
        Vector3 src = Vector3::UNIT_Z;
        mAnimatedNode->rotate(src.getRotationTo(path));
        mAnimatedNode->pitch(-Degree(flipAngle));
        return true; // Animation still running.
    }
    else if (mEndDelay >= 0)
    {
        if (!mCameraShaken)
        {
            Camera* camera = mSceneMgr->getCameraIterator().getNext();
            mAnimationManager->addAnimation(
                AnimationFactory::createCameraShakeAnimation(
                camera, mSceneMgr));
            mCameraShaken = true;
            mAnimatedNode->setPosition(mDestination);
        }

        mEndDelay -= timeSinceLastFrame;

        if (!mTargetPiece)
        {
            mEndDelay -= END_DELAY;
        }

        return true;
    }

    mAnimatedNode->setPosition(mDestination);
    mAnimatedNode->setOrientation(mAnimatedNode->getInitialOrientation());
    return false; // Animation finished.
}
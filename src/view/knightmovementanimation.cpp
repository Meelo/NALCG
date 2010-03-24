#include "knightmovementanimation.h"

bool KnightMovementAnimation::animate(const Real& timeSinceLastFrame)
{
    Real distanceMoved = MOVEMENT_SPEED * timeSinceLastFrame;
    Vector3 path = mDestination - mAnimatedNode->getPosition();

    if (path.length() > distanceMoved)
    {

        // Normalising the vector so the speed remains constant.
        path.normalise();
        mAnimatedNode->translate(path * distanceMoved);

        Vector3 src = mAnimatedNode->getOrientation() * Vector3::UNIT_Z;
        mAnimatedNode->rotate(src.getRotationTo(path));
        return true; // Animation still running.
    }

    mAnimatedNode->setPosition(mDestination);
    mAnimatedNode->setOrientation(mAnimatedNode->getInitialOrientation());
    return false; // Animation finished.
}
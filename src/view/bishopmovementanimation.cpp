#include "bishopmovementanimation.h"

bool BishopMovementAnimation::animate(const Real& timeSinceLastFrame)
{
    Real distanceMoved = MOVEMENT_SPEED * timeSinceLastFrame;
    Vector3 path = mDestination - mAnimatedNode->getPosition();

    if (path.length() > distanceMoved)
    {
        if (path.length() < 100 && mAttackDuration > 0)
        {
            if (!mParticleNode)
            {
                createBlasts();
            }
            mAttackDuration -= timeSinceLastFrame;
        }
        else {
            if (mParticleNode)
            {
                mAnimatedNode->removeAndDestroyChild(mParticleNode->getName());
                mParticleNode = 0;
            }
            // Normalising the vector so the speed remains constant.
            path.normalise();
            mAnimatedNode->translate(path * distanceMoved);

            Vector3 src = mAnimatedNode->getOrientation() * Vector3::UNIT_Z;
            mAnimatedNode->rotate(src.getRotationTo(path));
        }
        return true; // Animation still running.
    }

    mAnimatedNode->setPosition(mDestination);
    mAnimatedNode->setOrientation(mAnimatedNode->getInitialOrientation());
    return false; // Animation finished.
}

void BishopMovementAnimation::createBlasts()
{
    // Create a bloodstorm
    ParticleSystem* pSys = mSceneMgr->createParticleSystem(
        nextName(), "Effects/Flame");
    mParticleNode = mAnimatedNode->createChildSceneNode();
    //const Vector3& position = pieceNode->getPosition();
    mParticleNode->translate(0, 150, 0);
    mParticleNode->attachObject(pSys);
    ParticleEmitter *emitter = pSys->getEmitter(0);
    emitter->setTimeToLive(mAttackDuration + 1);
    pSys->setParticleQuota(emitter->getEmissionRate() * mAttackDuration);
}
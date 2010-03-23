#include "bishopmovementanimation.h"
#include "animationfactory.h"
#include "dyinganimation.h"

bool BishopMovementAnimation::animate(const Real& timeSinceLastFrame)
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
                    mTargetPiece, mSceneMgr, 2));
                mAnimationManager->addAnimation(
                    AnimationFactory::createBleedingAnimation(
                    mTargetPiece, mSceneMgr, 0.8, 2.5, "Effects/Burn"));
                mAnimationManager->addAnimation(
                    AnimationFactory::createBleedingAnimation(
                    mTargetPiece, mSceneMgr, 0.8, 3, "Effects/Smoke"));
            }
            if (mAttackDuration < 4 && mSceneMgr->hasSceneNode(mTargetPieceName))
            {
                mTargetPiece->yaw(Degree(timeSinceLastFrame * 80));
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
    mParticleNode->translate(-90, 270, 90);
    mParticleNode->attachObject(pSys);
    ParticleEmitter *emitter = pSys->getEmitter(0);
    //emitter->setTimeToLive(mAttackDuration + 1);
    pSys->setParticleQuota(emitter->getEmissionRate() * mAttackDuration);
}
#include "kingmovementanimation.h"

#include "animationfactory.h"
#include "dyinganimation.h"
#include "bleedinganimation.h"

bool KingMovementAnimation::animate(const Real& timeSinceLastFrame)
{
    Real distanceMoved = MOVEMENT_SPEED * timeSinceLastFrame;
    Vector3 path = mDestination - mAnimatedNode->getPosition();
    path.y = 0;
    Real verticalVelocity = (path.length() - mHalfTotalDistance) * mFlyingAltitude;

    if (path.length() > distanceMoved || mParticleNode)
    {
        switch (mPhase)
        {
            case 1:
                if (verticalVelocity <= 0)
                {
                    mPhase = 2;
                }
                break;
            case 2:
                if (mAttackDuration >= 0)
                {
                    if (!mParticleNode)
                    {
                        createBlasts();
                        mAnimationManager->addAnimation(
                            AnimationFactory::createDyingAnimation(
                            mTargetPiece, mSceneMgr, 2, 3));
                        mAnimationManager->addAnimation(
                            AnimationFactory::createBleedingAnimation(
                            mTargetPiece, mSceneMgr, 0.2, 2.5));
                        mAnimationManager->addAnimation(
                            AnimationFactory::createBleedingAnimation(
                            mTargetPiece, mSceneMgr, 0.2, 2, "Effects/Smoke"));
                    }

                    distanceMoved = 0;
                    mAttackDuration -= timeSinceLastFrame;
                }
                else
                {
                    mPhase = 3;
                    if (mParticleNode)
                    {
                        SceneNode::ObjectIterator itr = mParticleNode->getAttachedObjectIterator();
                        while (itr.hasMoreElements())
                        {
                            MovableObject* object = itr.getNext();
                            mSceneMgr->destroyMovableObject(object);
                        }
                        mAnimatedNode->removeAndDestroyChild(mParticleNode->getName());
                        mParticleNode = 0;
                    }
                }
                break;
        }


        if (distanceMoved > 0)
        {
            mAnimatedNode->translate(Vector3(0, verticalVelocity * timeSinceLastFrame, 0)); 

            Vector3 realPath = path;

            // Normalising the vector so the speed remains constant.
            path.normalise();
            mAnimatedNode->translate(path * distanceMoved);

            mAnimatedNode->resetOrientation();
            Vector3 src = Vector3::UNIT_Z;
            mAnimatedNode->rotate(src.getRotationTo(path));
            
            if (mTargetPiece)
            {
                Real pitch = realPath.length() * 45 / mHalfTotalDistance;
                if (mPhase == 1)
                {
                    pitch = 90 - pitch;
                }
                mAnimatedNode->pitch(Degree(pitch));
            }

            mAnimatedNode->yaw(Degree(realPath.length() * 360 / mHalfTotalDistance));

        }
        return true; // Animation still running.
    }

    mAnimatedNode->setPosition(mDestination);
    mAnimatedNode->setOrientation(mAnimatedNode->getInitialOrientation());
    return false; // Animation finished.
}

void KingMovementAnimation::createBlasts()
{
    ParticleSystem* pSys = mSceneMgr->createParticleSystem(
        nextName(), "Effects/EnergyBlast");
    mParticleNode = mAnimatedNode->createChildSceneNode();

    mParticleNode->translate(0, 160, 60);
    mParticleNode->attachObject(pSys);
    ParticleEmitter *emitter = pSys->getEmitter(0);

    pSys->setParticleQuota(emitter->getEmissionRate() * mAttackDuration);
}
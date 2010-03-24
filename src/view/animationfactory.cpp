#include "animationfactory.h"
#include "bishopmovementanimation.h"
#include "queenmovementanimation.h"
#include "knightmovementanimation.h"
#include "dyinganimation.h"
#include "CameraShakeAnimation.h"

MovementAnimation* AnimationFactory::createMovementAnimation(
    const char type, const Vector3& destination, SceneNode *movingNode,
    SceneNode *targetPiece, SceneManager *sceneMgr,
    AnimationManager *animationManager)
{
    switch (type)
    {
    case 'B':
        return new BishopMovementAnimation(destination, movingNode, targetPiece, sceneMgr, animationManager);
    case 'Q':
        return new QueenMovementAnimation(destination, movingNode, targetPiece, sceneMgr, animationManager);
    case 'N':
        return new KnightMovementAnimation(destination, movingNode, targetPiece, sceneMgr, animationManager);
    default:
        // TODO: change this to return 0 for testing when everything should be done.
        return new BishopMovementAnimation(destination, movingNode, targetPiece, sceneMgr, animationManager);
    }
}

BleedingAnimation* AnimationFactory::createBleedingAnimation(
    SceneNode *pieceNode, SceneManager *sceneMgr, const Real& delay,
    const Real& duration, const std::string& effectName)
{
    // Create a bloodstorm
    ParticleSystem* pSys = sceneMgr->createParticleSystem(
        GenericAnimation::nextName(), effectName);
    SceneNode* particleNode = pieceNode->createChildSceneNode();
    //const Vector3& position = pieceNode->getPosition();
    particleNode->translate(0, 150, 0);
    particleNode->attachObject(pSys);
    ParticleEmitter *emitter = pSys->getEmitter(0);
    emitter->setTimeToLive(duration + 3);
    emitter->setStartTime(delay + 0.01);
    pSys->setParticleQuota(emitter->getEmissionRate() * duration);

    return new BleedingAnimation(pieceNode, sceneMgr, duration + 3,
        particleNode, delay);
}

DyingAnimation* AnimationFactory::createDyingAnimation(SceneNode *pieceNode,
    SceneManager *sceneMgr, double delay, double dyingSpeedMultiplier)
{
    return new DyingAnimation(pieceNode, sceneMgr, delay, dyingSpeedMultiplier);
}

CameraShakeAnimation* AnimationFactory::createCameraShakeAnimation(Camera* camera, SceneManager* sceneMgr)
{
    return new CameraShakeAnimation(camera, sceneMgr);
}

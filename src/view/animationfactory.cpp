#include "animationfactory.h"
#include "bishopmovementanimation.h"
#include "queenmovementanimation.h"

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
    default:
        // TODO: change this to return 0 for testing when everything should be done.
        return new BishopMovementAnimation(destination, movingNode, targetPiece, sceneMgr, animationManager);
    }
}

BleedingAnimation* AnimationFactory::createBleedingAnimation(
    SceneNode *pieceNode, SceneManager *sceneMgr, const Real& duration)
{
    // Create a bloodstorm
    ParticleSystem* pSys = sceneMgr->createParticleSystem(
        GenericAnimation::nextName(), "Effects/Blood");
    SceneNode* particleNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
    const Vector3& position = pieceNode->getPosition();
    particleNode->translate(position.x, 150, position.z);
    particleNode->attachObject(pSys);
    ParticleEmitter *emitter = pSys->getEmitter(0);
    emitter->setTimeToLive(duration + 1);
    pSys->setParticleQuota(emitter->getEmissionRate() * duration);

    return new BleedingAnimation(particleNode, sceneMgr, duration + 1);
}


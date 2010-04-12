#include "animationfactory.h"

#include "bishopmovementanimation.h"
#include "queenmovementanimation.h"
#include "knightmovementanimation.h"
#include "dyinganimation.h"
#include "camerashakeanimation.h"
#include "kingmovementanimation.h"
#include "pawnmovementanimation.h"
#include "rookmovementanimation.h"
#include "bleedinganimation.h"
#include "promotionanimation.h"
#include "checkanimation.h"

MovementAnimation* AnimationFactory::createMovementAnimation(
    const char type, const Vector3& destination, SceneNode *movingNode,
    SceneNode *targetPiece, SceneManager *sceneMgr,
    AnimationManager *animationManager)
{
    switch (type)
    {
    case 'P':
        return new PawnMovementAnimation(destination, movingNode, targetPiece, sceneMgr, animationManager);
    case 'B':
        return new BishopMovementAnimation(destination, movingNode, targetPiece, sceneMgr, animationManager);
    case 'Q':
        return new QueenMovementAnimation(destination, movingNode, targetPiece, sceneMgr, animationManager);
    case 'N':
        return new KnightMovementAnimation(destination, movingNode, targetPiece, sceneMgr, animationManager);
    case 'K':
        return new KingMovementAnimation(destination, movingNode, targetPiece, sceneMgr, animationManager);
    case 'R':
        return new RookMovementAnimation(destination, movingNode, targetPiece, sceneMgr, animationManager);
    default:
        return 0;
    }
}

BleedingAnimation* AnimationFactory::createBleedingAnimation(
    SceneNode *pieceNode, SceneManager *sceneMgr, const Real& delay,
    const Real& duration, const std::string& effectName)
{
    ParticleSystem* pSys = sceneMgr->createParticleSystem(
        GenericAnimation::nextName(), effectName);
    SceneNode* particleNode = pieceNode->createChildSceneNode();

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

PromotionAnimation* AnimationFactory::createPromotionAnimation(
    unsigned int promoteTo, SceneNode *movingNode, SceneManager *sceneMgr, View* view)
{
    return new PromotionAnimation(promoteTo, movingNode, sceneMgr, view);
}

CheckAnimation* AnimationFactory::createCheckAnimation(
    SceneNode *checkedNode, SceneManager *sceneMgr)
{
    return new CheckAnimation(checkedNode, sceneMgr);
}
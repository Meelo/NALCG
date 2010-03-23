#pragma once

#include <Ogre.h>

#include "bleedinganimation.h"

using namespace Ogre;

class MovementAnimation;
class AnimationManager;
class DyingAnimation;
class AnimationFactory
{
public:
    static MovementAnimation* createMovementAnimation(const char type,
        const Vector3& destination, SceneNode *movingNode,
        SceneNode *targetPiece, SceneManager *sceneMgr,
        AnimationManager *animationManager);

    static BleedingAnimation* createBleedingAnimation(SceneNode *pieceNode,
        SceneManager *sceneMgr, const Real& delay, const Real& duration, 
        const std::string& effectName = "Effects/Blood");

    static DyingAnimation* createDyingAnimation(SceneNode *pieceNode,
        SceneManager *sceneMgr, double delay);
};
#ifndef _NALCG_ANIMATION_FACTORY_H_
#define _NALCG_ANIMATION_FACTORY_H_

#include <Ogre.h>

using namespace Ogre;

class MovementAnimation;
class AnimationManager;
class DyingAnimation;
class CameraShakeAnimation;
class BleedingAnimation;
class PromotionAnimation;
class View;
class CheckAnimation;
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
        SceneManager *sceneMgr, double delay, double dyingSpeedMultiplier = 1.0);

    static CameraShakeAnimation* createCameraShakeAnimation(Camera* camera, SceneManager* sceneMgr);

    static PromotionAnimation* createPromotionAnimation(
        unsigned int promoteTo, SceneNode *movingNode, SceneManager *sceneMgr, View* view);

    static CheckAnimation* createCheckAnimation(
        SceneNode *checkedNode, SceneManager *sceneMgr);
};

#endif // _NALCG_ANIMATION_FACTORY_H_

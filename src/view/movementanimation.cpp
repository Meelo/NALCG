#include "movementanimation.h"
#include "viewconstants.h"

void MovementAnimation::dimLights()
{
    mSceneMgr->setAmbientLight(ColourValue(0.05, 0.05, 0.05));
    mSceneMgr->getLight("Yellow")->setDiffuseColour(0.05, 0.05, 0);
    mSceneMgr->getLight("Blue")->setDiffuseColour(0.0, 0.0, 0.05);
}

void MovementAnimation::restoreLights()
{
    mSceneMgr->setAmbientLight(ViewConstants::AMBIENT_COLOUR);
    mSceneMgr->getLight("Yellow")->setDiffuseColour(ViewConstants::YELLOW_COLOUR);
    mSceneMgr->getLight("Blue")->setDiffuseColour(ViewConstants::BLUE_COLOUR);
}

void MovementAnimation::playAnimation(const std::string& animationName, double time, bool stop, SceneNode* animatedNode, bool loop)
{
    if (!animatedNode)
    {
        animatedNode = mAnimatedNode;
    }
    SceneNode::ObjectIterator it = animatedNode->getAttachedObjectIterator();
    while (it.hasMoreElements())
    {
        MovableObject* obj = it.getNext();
        Entity* ent = dynamic_cast<Entity*>(obj);

        AnimationStateSet* animations = ent->getAllAnimationStates();
        if (animations)
        {
            AnimationState* animationState = animations->getAnimationState(animationName);
            animationState->setEnabled(true);
            animationState->setLoop(loop);
            animationState->addTime(time);
            if (stop)
            {
                animationState->setTimePosition(0);
            }
        }
    }
}
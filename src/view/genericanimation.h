#pragma once

#include <Ogre.h>

using namespace Ogre;

class GenericAnimation
{
public:
    GenericAnimation(SceneNode *animatedNode, SceneManager *sceneMgr)
        : mAnimatedNode(animatedNode), mSceneMgr(sceneMgr)
    {
    }
    virtual ~GenericAnimation() { }

    virtual bool animate(const Real& timeSinceLastFrame) = 0;
    virtual SceneNode *getAnimatedNode() const { return mAnimatedNode; }
    static std::string nextName()
    {
        std::ostringstream name;
        name << "NALCG" << id++;
        return name.str();
    }

protected:
    SceneNode *mAnimatedNode;
    SceneManager *mSceneMgr;
    static int id;
};
#ifndef _NALCG_GENERIC_ANIMATION_H_
#define _NALCG_GENERIC_ANIMATION_H_

#include <Ogre.h>

using namespace Ogre;

class BufferedInputHandler;
class GenericAnimation
{
public:
    GenericAnimation(SceneNode *animatedNode, SceneManager *sceneMgr)
        : mAnimatedNode(animatedNode), mSceneMgr(sceneMgr)
    {
    }
    virtual ~GenericAnimation();

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

#endif // _NALCG_GENERIC_ANIMATION_H_

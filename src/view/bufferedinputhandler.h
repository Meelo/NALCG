#ifndef _NALCG_BUFFERED_INPUT_HANDLER_H_
#define _NALCG_BUFFERED_INPUT_HANDLER_H_

#include <Ogre.h>
#include <CEGUI/CEGUI.h>

using namespace Ogre;

class AnimationManager;
class BufferedInputHandler : public OIS::KeyListener, public OIS::MouseListener
{
public:
    BufferedInputHandler(RenderWindow *window, Camera *camera, SceneManager *sceneMgr,
        AnimationManager *animationManager)
        : mLMouseDown(false), mRMouseDown(false), mWindow(window), mCamera(camera),
        mSceneMgr(sceneMgr), mRaySceneQuery(sceneMgr->createRayQuery(Ray())),
        mSelectedObject(0), mDirection(Vector3::ZERO), mAnimationManager(animationManager)
    {
    }

    ~BufferedInputHandler()
    {
        mSceneMgr->destroyQuery(mRaySceneQuery);
    }

    // KeyListener
    virtual bool keyPressed(const OIS::KeyEvent &arg);
    virtual bool keyReleased(const OIS::KeyEvent &arg);
    CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID);
    virtual bool mouseMoved(const OIS::MouseEvent &arg);
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual void moveCamera(const Real& timeSinceLastFrame);

protected:
    bool mLMouseDown, mRMouseDown;     // True if the mouse buttons are down
    RenderWindow *mWindow;
    Camera *mCamera;
    SceneManager *mSceneMgr;           // A pointer to the scene manager
    RaySceneQuery *mRaySceneQuery;     // The ray scene query pointer
    SceneNode *mSelectedObject;         // The selected object
    Vector3 mDirection;     // Value to move in the correct direction
    AnimationManager *mAnimationManager;
    static const int CAMERA_MOVEMENT_SPEED = 500;


    virtual void onLeftPressed(const OIS::MouseEvent &arg);
    virtual SceneNode* findPieceAbove(SceneNode* squareNode);
    virtual void onLeftReleased(const OIS::MouseEvent &arg);
    virtual void onRightPressed(const OIS::MouseEvent &arg);
    virtual void onRightReleased(const OIS::MouseEvent &arg);
    virtual void showMovementPossibilities();
};

#endif // _NALCG_BUFFERED_INPUT_HANDLER_H_

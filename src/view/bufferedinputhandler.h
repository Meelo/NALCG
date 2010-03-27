#pragma once

#include <Ogre.h>
#include <CEGUI/CEGUI.h>

using namespace Ogre;

class View;
class AnimationManager;
class BufferedInputHandler : public OIS::KeyListener, public OIS::MouseListener
{
public:
    BufferedInputHandler(RenderWindow* window, Camera* camera, SceneManager* sceneMgr,
        AnimationManager* animationManager, View* view)
        : mLMouseDown(false), mRMouseDown(false), mWindow(window), mCamera(camera),
        mSceneMgr(sceneMgr), mRaySceneQuery(sceneMgr->createRayQuery(Ray())),
        mSelectedObject(0), mDirection(Vector3::ZERO), mAnimationManager(animationManager),
        mView(view)
    {
    }

    ~BufferedInputHandler()
    {
        mSceneMgr->destroyQuery(mRaySceneQuery);
    }

    // KeyListener
    virtual bool keyPressed(const OIS::KeyEvent& arg);
    virtual bool keyReleased(const OIS::KeyEvent& arg);
    virtual CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID) const;
    virtual bool mouseMoved(const OIS::MouseEvent& arg);
    virtual bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
    virtual void moveCamera(const Real& timeSinceLastFrame);

protected:
    bool mLMouseDown, mRMouseDown;     // True if the mouse buttons are down
    RenderWindow* mWindow;
    Camera* mCamera;
    SceneManager* mSceneMgr;           // A pointer to the scene manager
    RaySceneQuery* mRaySceneQuery;     // The ray scene query pointer
    SceneNode* mSelectedObject;         // The selected object
    Vector3 mDirection;     // Value to move in the correct direction
    AnimationManager* mAnimationManager;
    View* mView;
    static const int CAMERA_MOVEMENT_SPEED = 500;


    virtual void onLeftPressed(const OIS::MouseEvent& arg);
    virtual SceneNode* findPieceAbove(SceneNode* squareNode) const;
    virtual void onLeftReleased(const OIS::MouseEvent& arg);
    virtual void onRightPressed(const OIS::MouseEvent& arg);
    virtual void onRightReleased(const OIS::MouseEvent& arg);
    virtual bool showMovementPossibilities();
    virtual void convertPosition(const Vector3& position, int* x, int* y);
};


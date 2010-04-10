#ifndef _NALCG_VIEW_FRAME_LISTENER_H_
#define _NALCG_VIEW_FRAME_LISTENER_H_

#include "animationmanager.h"
#include "bufferedinputhandler.h"

#include <Ogre.h>
#include <CEGUI/CEGUI.h>
#include <OIS/OIS.h>

using namespace Ogre;

class ViewFrameListener : public FrameListener
{
public:
    ViewFrameListener(OIS::Keyboard* keyboard, OIS::Mouse* mouse,
        RenderWindow* window, Camera* camera, SceneManager* sceneManager,
        View* view)
        : mKeyboard(keyboard), mMouse(mouse), mAnimationManager(),
        mHandler(window, camera, sceneManager, &mAnimationManager, view),
        mContinue(true), mWindow(window), mTime(0), mSceneMgr(sceneManager),
        mCamera(camera), mCreated(false)
    {
        mDebugOverlay = OverlayManager::getSingleton().getByName("Core/DebugOverlay");
        mKeyboard->setEventCallback(&mHandler);
        mMouse->setEventCallback(&mHandler);
    }

    bool frameStarted(const FrameEvent& evt);
    bool frameEnded(const FrameEvent& evt);
    bool quit(const CEGUI::EventArgs& e);
    bool toggleDebugInfo(const CEGUI::EventArgs& e);
    bool handleAnimationSpeedChanged(const CEGUI::EventArgs& e);
    void move(int fromX, int fromY, int toX, int toY,
        bool continuous, unsigned int promoteTo = 0)
    {
        mHandler.move(fromX, fromY, toX, toY, continuous, promoteTo);
    }
    AnimationManager& getAnimationManager()
    {
        return mAnimationManager;
    }
    void setCanShowSelectablePieces(bool canShow)
    {
        mHandler.setCanShowSelectablePieces(canShow);
    }
    virtual void clearSelectedObject()
    {
        mHandler.clearSelectedObject();
    }
    virtual BufferedInputHandler* getHandler()
    {
        return &mHandler;
    }

protected:
    OIS::Keyboard* mKeyboard;
    OIS::Mouse* mMouse;
    AnimationManager mAnimationManager;
    BufferedInputHandler mHandler;
    bool mContinue;
    Overlay* mDebugOverlay;
    RenderWindow* mWindow;
    double mTime;
    SceneManager* mSceneMgr;
    Camera* mCamera;
    bool mCreated;

    virtual void updateStats();
    void flashMovableSquares(const Real& timeSinceLastFrame);
    void moveLights(const Real& timeSinceLastFrame);
    void playOpeningAnimation(const Real& timeSinceLastFrame);
};

#endif // _NALCG_VIEW_FRAME_LISTENER_H_

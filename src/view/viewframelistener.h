#ifndef _NALCG_VIEW_FRAME_LISTENER_H_
#define _NALCG_VIEW_FRAME_LISTENER_H_

#include <Ogre.h>
#include <CEGUI/CEGUI.h>
#include <OIS/OIS.h>

#include "animationmanager.h"
#include "bufferedinputhandler.h"

using namespace Ogre;

class ViewFrameListener : public FrameListener
{
public:
    ViewFrameListener(OIS::Keyboard* keyboard, OIS::Mouse* mouse,
        RenderWindow* window, Camera* camera, SceneManager* sceneManager)
        : mKeyboard(keyboard), mMouse(mouse), mAnimationManager(),
        mHandler(window, camera, sceneManager, &mAnimationManager),
        mContinue(true), mWindow(window)
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

protected:
    OIS::Keyboard* mKeyboard;
    OIS::Mouse* mMouse;
    AnimationManager mAnimationManager;
    BufferedInputHandler mHandler;
    bool mContinue;
    Overlay* mDebugOverlay;
    RenderWindow* mWindow;

    virtual void updateStats();
};

#endif // _NALCG_VIEW_FRAME_LISTENER_H_

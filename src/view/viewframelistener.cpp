#include "viewframelistener.h"

#include "viewconstants.h"

bool ViewFrameListener::frameStarted(const FrameEvent& evt)
{
    if (!mContinue)
    {
        return false;
    }
    mKeyboard->capture();
    mMouse->capture();

    mHandler.moveCamera(evt.timeSinceLastFrame);
    flashMovableSquares(evt.timeSinceLastFrame);
    moveLights(evt.timeSinceLastFrame);
    playOpeningAnimation(evt.timeSinceLastFrame);

    mAnimationManager.executeAnimations(evt.timeSinceLastFrame);
    return !mKeyboard->isKeyDown(OIS::KC_ESCAPE);
}

bool ViewFrameListener::frameEnded(const FrameEvent& evt)
{
    updateStats();
    return true;
}

void ViewFrameListener::flashMovableSquares(const Real& timeSinceLastFrame)
{
    mTime += timeSinceLastFrame;
    MaterialPtr mat = (MaterialPtr)MaterialManager::getSingleton().getByName("board/square/move");
    mat->setDiffuse(0, 1.0, 0, sin(mTime * 3) * 0.25 + 0.5);
}

void ViewFrameListener::moveLights(const Real& timeSinceLastFrame)
{
    double lightX = sin(mTime / 2.0) * 5200;
    double lightZ = cos(mTime / 2.0) * 5200;
    Light* light = mSceneMgr->getLight("Blue");
    light->setPosition(-lightX, 2500, -lightZ);

    light = mSceneMgr->getLight("Yellow");
    light->setPosition(lightX, 2500, lightZ);
}

void ViewFrameListener::playOpeningAnimation(const Real& timeSinceLastFrame)
{
    if (mTime < 4.5)
    {
        Vector3 adjustment = 2 * timeSinceLastFrame * ViewConstants::WHITE_CAMERA_POSITION;
        mCamera->setPosition(mCamera->getPosition() - adjustment);

        if (!mCreated && mTime > 2)
        {
            mSceneMgr->getSceneNode("ground")->setVisible(true);

#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            if (mWindow->isFullScreen())
#endif
            {
                CEGUI::MouseCursor::getSingleton().setVisible(true);
            }

            mSceneMgr->setSkyDome(true, "Sky", 10, 4);
            mCreated = true;
        }
    }
}

bool ViewFrameListener::quit(const CEGUI::EventArgs& e)
{
    mContinue = false;
    return true;
}

bool ViewFrameListener::toggleDebugInfo(const CEGUI::EventArgs& e)
{
    if (mDebugOverlay->isVisible())
    {
        mDebugOverlay->hide();
    }
    else
    {
        mDebugOverlay->show();
    }
    return true;
}

bool ViewFrameListener::handleAnimationSpeedChanged(const CEGUI::EventArgs& e)
{
    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window* text = wmgr.getWindow("View/Animation speed: 1xStaticText");

    CEGUI::Window* window = wmgr.getWindow("View/AnimationSpeedSlider");
    CEGUI::Scrollbar* scrollbar = static_cast<CEGUI::Scrollbar*>(window);
    float position = scrollbar->getScrollPosition();
    float multiplier = position;
    if (position >= scrollbar->getDocumentSize())
    {
        multiplier = std::numeric_limits<float>::max();
        text->setText("Animation speed: Instant");
    }
    else if (position > 1)
    {
        multiplier = Math::Pow(position, 4) * 0.08 + 0.92;
    }
    mAnimationManager.setAnimationSpeedMultiplier(multiplier);

    if (position < scrollbar->getDocumentSize())
    {
        text->setText("Animation speed: " + StringConverter::toString(
            int(multiplier * 100 + 0.5) / 100.0f) + "x");
    }
    return true;
}

void ViewFrameListener::updateStats()
{
    static String currFps = "Current FPS: ";
    static String avgFps = "Average FPS: ";
    static String bestFps = "Best FPS: ";
    static String worstFps = "Worst FPS: ";
    static String tris = "Triangle Count: ";
    static String batches = "Batch Count: ";

    // update stats when necessary
    try {
        OverlayElement* guiAvg = OverlayManager::getSingleton().getOverlayElement("Core/AverageFps");
        OverlayElement* guiCurr = OverlayManager::getSingleton().getOverlayElement("Core/CurrFps");
        OverlayElement* guiBest = OverlayManager::getSingleton().getOverlayElement("Core/BestFps");
        OverlayElement* guiWorst = OverlayManager::getSingleton().getOverlayElement("Core/WorstFps");

        const RenderTarget::FrameStats& stats = mWindow->getStatistics();
        guiAvg->setCaption(avgFps + StringConverter::toString(stats.avgFPS));
        guiCurr->setCaption(currFps + StringConverter::toString(stats.lastFPS));
        guiBest->setCaption(bestFps + StringConverter::toString(stats.bestFPS)
            +" "+StringConverter::toString(stats.bestFrameTime)+" ms");
        guiWorst->setCaption(worstFps + StringConverter::toString(stats.worstFPS)
            +" "+StringConverter::toString(stats.worstFrameTime)+" ms");

        OverlayElement* guiTris = OverlayManager::getSingleton().getOverlayElement("Core/NumTris");
        guiTris->setCaption(tris + StringConverter::toString(stats.triangleCount));

        OverlayElement* guiBatches = OverlayManager::getSingleton().getOverlayElement("Core/NumBatches");
        guiBatches->setCaption(batches + StringConverter::toString(stats.batchCount));

        //OverlayElement* guiDbg = OverlayManager::getSingleton().getOverlayElement("Core/DebugText");
        //guiDbg->setCaption(mDebugText);
    }
    catch(...) { /* ignore */ }
}


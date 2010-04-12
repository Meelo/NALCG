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

    if (!mAnimationManager.animationsRunning())
    {
        if (mHandler.canShowSelectablePieces()
            && mHandler.getMoveAssistanceLevel() > 2)
        {
            mHandler.showSelectablePieces();
        }
        if (mHandler.getMoveAssistanceLevel() > 0)
        {
            mHandler.highlightHoveredSquare();
        }
    }

    SceneNode* invalidMoveNode = mSceneMgr->getSceneNode("InvalidMove");
    Vector3 scale = invalidMoveNode->getScale();
    scale.y += evt.timeSinceLastFrame * 0.1;

    if (scale.y < 1.1)
    {
        scale.x = sin((scale.y - 1.0) * Math::PI / 0.1);
        scale.z = scale.x;
        invalidMoveNode->setScale(scale);
    }
    else
    {
        invalidMoveNode->setVisible(false);
    }

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

    MaterialPtr moveMat = (MaterialPtr)MaterialManager::getSingleton().getByName("board/square/move");
    MaterialPtr attackMat = (MaterialPtr)MaterialManager::getSingleton().getByName("board/square/attack");

    if (mHandler.getMoveAssistanceLevel() > 1)
    {
        moveMat->setDiffuse(0, 1.0, 0, sin(mTime * 3) * 0.25 + 0.5);
        attackMat->setDiffuse(1.0, 1.0, 0, sin(mTime * 3) * 0.25 + 0.5);
    }
    else
    {
        moveMat->setDiffuse(0, 1.0, 0, 0);
        attackMat->setDiffuse(1.0, 1.0, 0, 0);
    }

    MaterialPtr mat = (MaterialPtr)MaterialManager::getSingleton().getByName("board/square/selected");
    mat->setDiffuse(0, 1.0, 1.0, sin(mTime * 3) * 0.1 + 0.4);

    mat = (MaterialPtr)MaterialManager::getSingleton().getByName("board/square/invalid");
    mat->setDiffuse(1.0, 0, 0, sin(mTime * 3) * 0.1 + 0.4);


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

bool ViewFrameListener::hideGUI(const CEGUI::EventArgs& e)
{
    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window* hideButton = wmgr.getWindow("View/" + ViewConstants::SHOW_ADDITIONAL + "Button");

    using ViewConstants::SHOW_ADDITIONAL;
    using ViewConstants::HIDE_GUI;
    using ViewConstants::SHOW_BASIC;

    if (hideButton->getText() == SHOW_ADDITIONAL)
    {
        hideButton->setText(HIDE_GUI);
        wmgr.getWindow("View/Animation speedStaticText")->setVisible(true);
        wmgr.getWindow("View/Animation speedHorizontalScrollbar")->setVisible(true);
        wmgr.getWindow("View/Unsafe modeCheckbox")->setVisible(false); // not in demo
        wmgr.getWindow("View/Move assistance levelStaticText")->setVisible(true);
        wmgr.getWindow("View/Move assistanceSpinner")->setVisible(true);
        wmgr.getWindow("View/FPS infoButton")->setVisible(true);
    }
    else if (hideButton->getText() == HIDE_GUI)
    {
        hideButton->setText(SHOW_BASIC);
        wmgr.getWindow("View/Animation speedStaticText")->setVisible(false);
        wmgr.getWindow("View/Animation speedHorizontalScrollbar")->setVisible(false);
        wmgr.getWindow("View/Unsafe modeCheckbox")->setVisible(false);
        wmgr.getWindow("View/Move assistance levelStaticText")->setVisible(false);
        wmgr.getWindow("View/Move assistanceSpinner")->setVisible(false);

        wmgr.getWindow("View/UndoButton")->setVisible(false);
        wmgr.getWindow("View/RestartButton")->setVisible(false);
        wmgr.getWindow("View/QuitButton")->setVisible(false);
        wmgr.getWindow("View/FPS infoButton")->setVisible(false); // remove from final product
        wmgr.getWindow("View/LogListbox")->setVisible(false);
        wmgr.getWindow("View/Game logStaticText")->setVisible(false);
    }
    else if (hideButton->getText() == SHOW_BASIC)
    {
        hideButton->setText(SHOW_ADDITIONAL);

        wmgr.getWindow("View/UndoButton")->setVisible(true);
        wmgr.getWindow("View/RestartButton")->setVisible(true);
        wmgr.getWindow("View/QuitButton")->setVisible(true);
        wmgr.getWindow("View/FPS infoButton")->setVisible(false); // remove from final product
        wmgr.getWindow("View/LogListbox")->setVisible(true);
        wmgr.getWindow("View/Game logStaticText")->setVisible(true);
    }
    return true;
}

bool ViewFrameListener::handleAnimationSpeedChanged(const CEGUI::EventArgs& e)
{
    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window* text = wmgr.getWindow("View/Animation speedStaticText");

    CEGUI::Window* window = wmgr.getWindow("View/Animation speedHorizontalScrollbar");
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


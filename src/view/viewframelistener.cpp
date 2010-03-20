#include "viewframelistener.h"

bool ViewFrameListener::frameStarted(const FrameEvent& evt)
{
    mKeyboard->capture();
    mMouse->capture();
    mHandler.moveCamera(evt.timeSinceLastFrame);
    mAnimationManager.executeAnimations(evt.timeSinceLastFrame);
    return mContinue && !mKeyboard->isKeyDown(OIS::KC_ESCAPE);
}

bool ViewFrameListener::frameEnded(const FrameEvent& evt)
{
    updateStats();
    return true;
}

bool ViewFrameListener::quit(const CEGUI::EventArgs &e)
{
    mContinue = false;
    return true;
}

bool ViewFrameListener::toggleDebugInfo(const CEGUI::EventArgs &e)
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


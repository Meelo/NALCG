#include <Ogre.h>
#include <OIS/OIS.h>
#include <CEGUI/CEGUI.h>
#include <OgreCEGUIRenderer.h>

using namespace Ogre;

namespace ViewConstants
{
    static const ColourValue AMBIENT_COLOUR(ColourValue(0.4, 0.4, 0.4));
    static const ColourValue YELLOW_COLOUR(ColourValue(1.0, 1.0, 0.4));
    static const ColourValue BLUE_COLOUR(ColourValue(0.4, 0.4, 1.0));
};

class MovementAnimation
{
public:
    MovementAnimation(const Vector3& destination) : mDestination(destination)
    {
    }

    virtual ~MovementAnimation()
    {
    }

    virtual bool animate(const Real& timeSinceLastFrame, SceneNode *movingNode) = 0;

protected:
    Vector3 mDestination;
};

class BishopMovementAnimation : public MovementAnimation
{
public:
    BishopMovementAnimation(const Vector3& destination)
        : MovementAnimation(destination)
    {
    }

    virtual bool animate(const Real& timeSinceLastFrame, SceneNode *movingNode)
    {
        Real distanceMoved = MOVEMENT_SPEED * timeSinceLastFrame;
        Vector3 path = mDestination - movingNode->getPosition();

        if (path.length() > distanceMoved)
        {
            // Normalising the vector so the speed remains constant.
            path.normalise();
            movingNode->translate(path * distanceMoved);

            Vector3 src = movingNode->getOrientation() * Vector3::UNIT_Z;
            movingNode->rotate(src.getRotationTo(path));
            return true; // Animation still running.
        }

        movingNode->setPosition(mDestination);
        movingNode->setOrientation(movingNode->getInitialOrientation());
        return false; // Animation finished.
    }
protected:
    static const int MOVEMENT_SPEED = 500;
};

class View;
class QueenMovementAnimation : public MovementAnimation
{
public:
    QueenMovementAnimation(const Vector3& destination, bool occupied, SceneManager *sceneMgr)
        : MovementAnimation(destination), mAttackDuration(occupied ? 1000 : -1),
        mPhase(1), mSceneMgr(sceneMgr), mTrail(0)
    {
        assert(sceneMgr != 0);
        createBlasts();
    }

    virtual bool animate(const Real& timeSinceLastFrame, SceneNode *movingNode)
    {
        Real distanceMoved = MOVEMENT_SPEED * timeSinceLastFrame;
        Vector3 path = mDestination - movingNode->getPosition();
        if (path.length() > distanceMoved)
        {

            bool flyHigher = movingNode->getPosition().y < 500;
            switch (mPhase)
            {
            case 1:
                // Fly higher in steep angle until flying altitude is reached.
                if (movingNode->getPosition().y > FLYING_ALTITUDE)
                {
                    mPhase = 2;
                }
                path += Vector3(0, FLYING_ALTITUDE * 4, 0);
                break;
            case 2:
                // Continue flying on the same altitude until almost above target.
                if (path.length() < distanceMoved + FLYING_ALTITUDE * 1.1)
                {
                    mPhase = 3;
                    dimLights();
                }
                path += Vector3(0, FLYING_ALTITUDE, 0);
                break;
            case 3:
                {
                    if (mAttackDuration < 0)
                    {
                        mPhase = 4;
                        restoreLights();
                    }

                    std::vector<AnimationState*>::iterator animi;
                    for (animi = mAnimStateList.begin(); animi != mAnimStateList.end(); ++animi)
                    {
                        (*animi)->addTime(timeSinceLastFrame);
                    }
                    path += Vector3(0, FLYING_ALTITUDE, 0);
                    mAttackDuration -= timeSinceLastFrame;
                    break;
                }
            case 4:
                break;
            }

            // Normalising the vector so the speed remains constant.
            path.normalise();
            movingNode->translate(path * distanceMoved);

            /*if (flyHigher)
            {
            movingNode->setOrientation(movingNode->getInitialOrientation());
            Vector3 src = movingNode->getOrientation() * Vector3::UNIT_Z;
            movingNode->rotate(src.getRotationTo(path));
            }*/

            return true; // Animation still running.
        }

        movingNode->setPosition(mDestination);
        movingNode->setOrientation(movingNode->getInitialOrientation());
        return false; // Animation finished.
    }

    virtual void createBlasts()
    {
        NameValuePairList pairList;
        pairList["numberOfChains"] = "5";
        pairList["maxElements"] = "80";

        std::ostringstream trailName;
        trailName << "Trail" << trailId;
        trailId++;

        mTrail = static_cast<RibbonTrail*>(
            mSceneMgr->createMovableObject(trailName.str(), "RibbonTrail", &pairList));
        mTrail->setMaterialName("Examples/LightRibbonTrail");
        mTrail->setTrailLength(400);

        mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(mTrail);

        // Create 3 nodes for trail to follow
        SceneNode* animNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        animNode->setPosition(50,30,0);
        Animation* anim = mSceneMgr->createAnimation("an1", 14);
        anim->setInterpolationMode(Animation::IM_SPLINE);
        NodeAnimationTrack* track = anim->createNodeTrack(1, animNode);
        TransformKeyFrame* kf = track->createNodeKeyFrame(0);
        kf->setTranslate(Vector3(50,30,0));
        kf = track->createNodeKeyFrame(2);
        kf->setTranslate(Vector3(100, 30, 0));
        kf = track->createNodeKeyFrame(4);
        kf->setTranslate(Vector3(120, 100, 150));
        kf = track->createNodeKeyFrame(6);
        kf->setTranslate(Vector3(30, 100, 50));
        kf = track->createNodeKeyFrame(8);
        kf->setTranslate(Vector3(-50, 130, -50));
        kf = track->createNodeKeyFrame(10);
        kf->setTranslate(Vector3(-150, 20, -100));
        kf = track->createNodeKeyFrame(12);
        kf->setTranslate(Vector3(-50, 30, 0));
        kf = track->createNodeKeyFrame(14);
        kf->setTranslate(Vector3(50,30,0));

        AnimationState* animState = mSceneMgr->createAnimationState("an1");
        animState->setEnabled(true);
        mAnimStateList.push_back(animState);

        mTrail->setInitialColour(0, 0.3, 0.5, 1.0);
        mTrail->setColourChange(0, 0.5, 0.5, 0.5, 0.5);
        mTrail->setInitialWidth(0, 5);
        mTrail->addNode(animNode);

        // Add light
        Light* l2 = mSceneMgr->createLight("l2");
        l2->setDiffuseColour(mTrail->getInitialColour(0));
        animNode->attachObject(l2);

        // Add billboard
        BillboardSet* bbs = mSceneMgr->createBillboardSet("bb", 1);
        bbs->createBillboard(Vector3::ZERO, mTrail->getInitialColour(0));
        bbs->setMaterialName("Examples/Flare");
        animNode->attachObject(bbs);
    }

    virtual void dimLights()
    {
        mSceneMgr->setAmbientLight(ColourValue(0.05, 0.05, 0.05));
        mSceneMgr->getLight("Yellow")->setDiffuseColour(0.05, 0.05, 0);
        mSceneMgr->getLight("Blue")->setDiffuseColour(0.0, 0.0, 0.05);
    }

    virtual void restoreLights()
    {
        mSceneMgr->setAmbientLight(ViewConstants::AMBIENT_COLOUR);
        mSceneMgr->getLight("Yellow")->setDiffuseColour(ViewConstants::YELLOW_COLOUR);
        mSceneMgr->getLight("Blue")->setDiffuseColour(ViewConstants::BLUE_COLOUR);
    }

protected:
    static const int MOVEMENT_SPEED = 500;
    static const int FLYING_ALTITUDE = 500;
    static int trailId;

    Real mAttackDuration;
    int mPhase;
    SceneManager *mSceneMgr;
    std::vector<AnimationState*> mAnimStateList;
    RibbonTrail* mTrail;
};

int QueenMovementAnimation::trailId = 1;

class MovementAnimationFactory
{
public:
    static MovementAnimation* createAnimation(const char type, const Vector3& destination, bool occupied, SceneManager *sceneMgr)
    {
        switch (type)
        {
        case 'B':
            return new BishopMovementAnimation(destination);
        case 'Q':
            return new QueenMovementAnimation(destination, occupied, sceneMgr);
        default:
            // TODO: change this to return 0 for testing when everything should be done.
            return new BishopMovementAnimation(destination);
        }
    }
};

class BufferedInputHandler : public OIS::KeyListener, public OIS::MouseListener
{
public:
    BufferedInputHandler(RenderWindow *window, Camera *camera, SceneManager *sceneMgr)
        : mLMouseDown(false), mRMouseDown(false), mWindow(window), mCamera(camera),
        mSceneMgr(sceneMgr), mRaySceneQuery(sceneMgr->createRayQuery(Ray())),
        mSelectedObject(0), mDirection(Vector3::ZERO)
    {
    }

    ~BufferedInputHandler()
    {
        mSceneMgr->destroyQuery(mRaySceneQuery);
    }

    // KeyListener
    virtual bool keyPressed(const OIS::KeyEvent &arg)
    {
        CEGUI::System *sys = CEGUI::System::getSingletonPtr();
        sys->injectKeyDown(arg.key);
        sys->injectChar(arg.text);

        switch (arg.key)
        {
        case OIS::KC_UP:
        case OIS::KC_W:
            mDirection.z -= CAMERA_MOVEMENT_SPEED;
            break;

        case OIS::KC_DOWN:
        case OIS::KC_S:
            mDirection.z += CAMERA_MOVEMENT_SPEED;
            break;

        case OIS::KC_LEFT:
        case OIS::KC_A:
            mDirection.x -= CAMERA_MOVEMENT_SPEED;
            break;

        case OIS::KC_RIGHT:
        case OIS::KC_D:
            mDirection.x += CAMERA_MOVEMENT_SPEED;
            break;

        case OIS::KC_PGDOWN:
        case OIS::KC_F:
            mDirection.y -= CAMERA_MOVEMENT_SPEED;
            break;

        case OIS::KC_PGUP:
        case OIS::KC_R:
            mDirection.y += CAMERA_MOVEMENT_SPEED;
            break;
        }
        return true;
    }

    virtual bool keyReleased(const OIS::KeyEvent &arg)
    {
        CEGUI::System::getSingleton().injectKeyUp(arg.key);

        switch (arg.key)
        {
        case OIS::KC_UP:
        case OIS::KC_W:
            mDirection.z += CAMERA_MOVEMENT_SPEED;
            break;

        case OIS::KC_DOWN:
        case OIS::KC_S:
            mDirection.z -= CAMERA_MOVEMENT_SPEED;
            break;

        case OIS::KC_LEFT:
        case OIS::KC_A:
            mDirection.x += CAMERA_MOVEMENT_SPEED;
            break;

        case OIS::KC_RIGHT:
        case OIS::KC_D:
            mDirection.x -= CAMERA_MOVEMENT_SPEED;
            break;

        case OIS::KC_PGDOWN:
        case OIS::KC_F:
            mDirection.y += CAMERA_MOVEMENT_SPEED;
            break;

        case OIS::KC_PGUP:
        case OIS::KC_R:
            mDirection.y -= CAMERA_MOVEMENT_SPEED;
            break;
        }
        return true;
    }

    // MouseListener

    CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
    {
        switch (buttonID)
        {
        case OIS::MB_Left:
            return CEGUI::LeftButton;

        case OIS::MB_Right:
            return CEGUI::RightButton;

        case OIS::MB_Middle:
            return CEGUI::MiddleButton;

        default:
            return CEGUI::LeftButton;
        }
    }
    virtual bool mouseMoved(const OIS::MouseEvent &arg)
    {
        if (!mRMouseDown) {
            CEGUI::System::getSingleton().injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
        }

        // If we are dragging the left mouse button.
        if (mLMouseDown)
        {

        }

        // If we are dragging the right mouse button.
        else if (mRMouseDown)
        {
            const double rotationSpeed = 0.2;
            mCamera->yaw(Degree(-arg.state.X.rel) * rotationSpeed);
            mCamera->pitch(Degree(-arg.state.Y.rel) * rotationSpeed);
        }

        return true;
    }
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
    {
        CEGUI::System::getSingleton().injectMouseButtonDown(convertButton(id));

        // Left mouse button down
        if (id == OIS::MB_Left)
        {
            onLeftPressed(arg);
            mLMouseDown = true;
        }

        // Right mouse button down
        else if (id == OIS::MB_Right)
        {
            onRightPressed(arg);
            mRMouseDown = true;
        }
        return true;
    }
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
    {
        CEGUI::System::getSingleton().injectMouseButtonUp(convertButton(id));

        // Left mouse button up
        if (id == OIS::MB_Left)
        {
            onLeftReleased(arg);
            mLMouseDown = false;
        }

        // Right mouse button up
        else if (id == OIS::MB_Right)
        {
            onRightReleased(arg);
            mRMouseDown = false;
        }
        return true;
    }

    virtual void executeActions(const Real& timeSinceLastFrame)
    {
        mCamera->setPosition(mCamera->getPosition() + 
            mCamera->getOrientation() * mDirection * timeSinceLastFrame);

        executeAnimations(timeSinceLastFrame);
    }

protected:
    bool mLMouseDown, mRMouseDown;     // True if the mouse buttons are down
    RenderWindow *mWindow;
    Camera *mCamera;
    SceneManager *mSceneMgr;           // A pointer to the scene manager
    RaySceneQuery *mRaySceneQuery;     // The ray scene query pointer
    SceneNode *mSelectedObject;         // The selected object
    Vector3 mDirection;     // Value to move in the correct direction
    static const int CAMERA_MOVEMENT_SPEED = 500;
    std::map<SceneNode*, MovementAnimation*> mMovementAnimations;
    //CEGUI::Renderer *mGUIRenderer;     // CEGUI renderer
    //bool mRobotMode;                   // The current state

    virtual void executeAnimations(const Real& timeSinceLastFrame)
    {
        std::vector<SceneNode*> removed;

        for (std::map<SceneNode*, MovementAnimation*>::iterator it =
            mMovementAnimations.begin(); it != mMovementAnimations.end(); it++)
        {
            if (!it->second->animate(timeSinceLastFrame, it->first))
            {
                removed.push_back(it->first);
                delete it->second;
            }
        }

        for (std::size_t i = 0; i < removed.size(); i++)
        {
            mMovementAnimations.erase(removed.at(i));
        }
    }

    virtual void onLeftPressed(const OIS::MouseEvent &arg)
    {
        CEGUI::Point mousePos = CEGUI::MouseCursor::getSingleton().getPosition();

        // Not using arg.state.width since it won't be updated.
        Ray mouseRay = mCamera->getCameraToViewportRay(mousePos.d_x/mWindow->getWidth(), mousePos.d_y/mWindow->getHeight());
        mRaySceneQuery->setRay(mouseRay);
        mRaySceneQuery->setSortByDistance(true);
        mRaySceneQuery->setQueryMask(1 << 0);

        RaySceneQueryResult &result = mRaySceneQuery->execute();
        RaySceneQueryResult::iterator itr;

        for (itr = result.begin(); itr != result.end(); itr++)
        {
            if (itr->movable)
            {
                if (mSelectedObject)
                {
                    SceneNode *targetNode = itr->movable->getParentSceneNode();
                    SceneNode *pieceNode = findPieceAbove(mSelectedObject);
                    if (mSelectedObject != targetNode)
                    {
                        std::cout << mSelectedObject->getName() << " -> " << targetNode->getName() << std::endl;

                        SceneNode *targetPiece = findPieceAbove(targetNode);
                        if (targetPiece)
                        {
                            mSceneMgr->getRootSceneNode()->removeAndDestroyChild(targetPiece->getName());
                        }

                        //pieceNode->setPosition(targetNode->getPosition());

                        mMovementAnimations[pieceNode] = 
                            MovementAnimationFactory::createAnimation(
                            *pieceNode->getName().begin(),
                            targetNode->getPosition(), targetPiece != 0,
                            mSceneMgr);
                    }
                    mSelectedObject->showBoundingBox(false);
                    pieceNode->showBoundingBox(false);
                    mSelectedObject = 0;
                }
                else
                {
                    SceneNode *squareNode = itr->movable->getParentSceneNode();
                    SceneNode *pieceNode = findPieceAbove(squareNode);
                    if (pieceNode)
                    {
                        mSelectedObject = squareNode;
                        mSelectedObject->showBoundingBox(true);
                        pieceNode->showBoundingBox(true);
                    }
                }
                break;
            }
        }
    }

    virtual SceneNode* findPieceAbove(SceneNode* squareNode)
    {
        const Vector3& squarePosition = squareNode->getPosition();
        Node::ChildNodeIterator it = mSceneMgr->getRootSceneNode()->getChildIterator();
        while (it.hasMoreElements())
        {
            Node* next = it.getNext();
            if (squareNode != next && next->getPosition() == squarePosition)
            {
                return dynamic_cast<SceneNode*>(next);
            }
        }
        return 0;
    }

    virtual void onLeftReleased(const OIS::MouseEvent &arg)
    {
    }

    virtual void onRightPressed(const OIS::MouseEvent &arg)
    {
    }

    virtual void onRightReleased(const OIS::MouseEvent &arg)
    {
    }
};

class ViewFrameListener : public FrameListener
{
public:
    ViewFrameListener(OIS::Keyboard *keyboard, OIS::Mouse *mouse,
        RenderWindow *window, Camera *camera, SceneManager *sceneManager)
        : mKeyboard(keyboard), mMouse(mouse), mHandler(window, camera, sceneManager),
        mContinue(true), mWindow(window)
    {
        mDebugOverlay = OverlayManager::getSingleton().getByName("Core/DebugOverlay");
        mKeyboard->setEventCallback(&mHandler);
        mMouse->setEventCallback(&mHandler);
    }

    bool frameStarted(const FrameEvent& evt)
    {
        mKeyboard->capture();
        mMouse->capture();
        mHandler.executeActions(evt.timeSinceLastFrame);
        return mContinue && !mKeyboard->isKeyDown(OIS::KC_ESCAPE);
    }

    bool frameEnded(const FrameEvent& evt)
    {
        updateStats();
        return true;
    }

    bool quit(const CEGUI::EventArgs &e)
    {
        mContinue = false;
        return true;
    }

    bool toggleDebugInfo(const CEGUI::EventArgs &e)
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

protected:
    OIS::Keyboard *mKeyboard;
    OIS::Mouse *mMouse;
    BufferedInputHandler mHandler;
    bool mContinue;
    Overlay* mDebugOverlay;
    RenderWindow *mWindow;

    virtual void updateStats(void)
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

};

class View
{
public:
    View() : mRoot(0), mKeyboard(0), mMouse(0), mInputManager(0),
        mRenderer(0), mSystem(0), mListener(0), mDecalFrustum(0),
        mFilterFrustum(0), mProjectorNode(0), mEntityCount(0)
    {
    }

    void go()
    {
        createRoot();
        defineResources();
        setupRenderSystem();
        createRenderWindow();
        initializeResourceGroups();
        setupScene();
        setupInputSystem();
        setupCEGUI();
        createFrameListener();
        createScene();
        startRenderLoop();
    }

    virtual ~View()
    {
        if (mInputManager)
        {
            mInputManager->destroyInputObject(mKeyboard);
            mInputManager->destroyInputObject(mMouse);
            OIS::InputManager::destroyInputSystem(mInputManager);
        }

        delete mSystem;
        delete mRenderer;

        delete mListener;
        delete mRoot;
    }
protected:
    Root *mRoot;
    OIS::Keyboard *mKeyboard;
    OIS::Mouse *mMouse;
    OIS::InputManager *mInputManager;
    CEGUI::OgreCEGUIRenderer *mRenderer;
    CEGUI::System *mSystem;
    ViewFrameListener *mListener;
    SceneManager *mSceneMgr;
    Camera *mCamera;
    RenderWindow *mWindow;
    SceneNode *mProjectorNode;
    Frustum *mDecalFrustum;
    Frustum *mFilterFrustum;

    int mEntityCount;

    void createRoot()
    {
#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        mRoot = new Root("plugins-windows.cfg");
#else
        mRoot = new Root("plugins-linux.cfg");
#endif
    }

    void defineResources()
    {
        String secName, typeName, archName;
        ConfigFile cf;
        cf.load("resources.cfg");
        ConfigFile::SectionIterator seci = cf.getSectionIterator();
        while (seci.hasMoreElements())
        {
            secName = seci.peekNextKey();
            ConfigFile::SettingsMultiMap *settings = seci.getNext();
            ConfigFile::SettingsMultiMap::iterator i;
            for (i = settings->begin(); i != settings->end(); ++i)
            {
                typeName = i->first;
                archName = i->second;
                ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
            }
        }
    }

    void setupRenderSystem()
    {
        if (!mRoot->restoreConfig() && !mRoot->showConfigDialog())
            throw Exception(52, "User canceled the config dialog!", "Application::setupRenderSystem()");
    }

    void createRenderWindow()
    {
        mRoot->initialise(true, "Not Another Lousy Chess Game");
    }

    void initializeResourceGroups()
    {
        TextureManager::getSingleton().setDefaultNumMipmaps(5);
        ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    }

    void setupScene()
    {
        mSceneMgr = mRoot->createSceneManager(ST_GENERIC, "Default SceneManager");
        createCamera();
        mWindow = mRoot->getAutoCreatedWindow();
        createViewports();
    }

    void setupInputSystem()
    {
        size_t windowHnd = 0;
        std::ostringstream windowHndStr;
        OIS::ParamList pl;

        mWindow->getCustomAttribute("WINDOW", &windowHnd);
        windowHndStr << windowHnd;
        pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
        mInputManager = OIS::InputManager::createInputSystem(pl);

        try
        {
            mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
            mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));
            //mJoy = static_cast<OIS::JoyStick*>(mInputManager->createInputObject(OIS::OISJoyStick, false));
        }
        catch (const OIS::Exception &e)
        {
            throw Exception(42, e.eText, "Application::setupInputSystem");
        }
    }

    void setupCEGUI()
    {
        mRenderer = new CEGUI::OgreCEGUIRenderer(mWindow, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, mSceneMgr);
        mSystem = new CEGUI::System(mRenderer);

        CEGUI::SchemeManager::getSingleton().loadScheme("TaharezLookSkin.scheme");

        mSystem->setDefaultMouseCursor("TaharezLook", "MouseArrow");
        mSystem->setDefaultFont("BlueHighway-12");

        CEGUI::MouseCursor::getSingleton().setImage(CEGUI::System::getSingleton().getDefaultMouseCursor());
    }

    void createFrameListener()
    {
        mListener = new ViewFrameListener(mKeyboard, mMouse, mWindow, mCamera, mSceneMgr);
        mRoot->addFrameListener(mListener);
    }

    void startRenderLoop()
    {
        mRoot->startRendering();
    }

    virtual void createCamera()
    {
        mCamera = mSceneMgr->createCamera("PlayerCam");
        // set its position, direction
        mCamera->setPosition(Vector3(0,1500,1500));
        mCamera->lookAt(Vector3(0,0,-45.0));
        mCamera->setNearClipDistance(5);
    }

    virtual void createViewports()
    {
        // Create one viewport, entire window
        Viewport* vp = mWindow->addViewport(mCamera);
        vp->setBackgroundColour(ColourValue(0,0,0));
        // Alter the camera aspect ratio to match the viewport
        mCamera->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
    }

    // The function to create our decal projector
    void createProjector()
    {
        mDecalFrustum = new Frustum();
        mProjectorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("DecalProjectorNode");
        mProjectorNode->attachObject(mDecalFrustum);
        mProjectorNode->setPosition(0,5,0);
    }

    // A function to take an existing material and make it receive the projected decal
    void makeMaterialReceiveDecal(const String &matName)
    {
    }

    virtual void createPiece(char type, const std::string& modelName,
        const Vector3& location)
    {
        std::ostringstream entityName;
        entityName << type << modelName << mEntityCount;
        mEntityCount++;

        Entity* ent = mSceneMgr->createEntity(entityName.str(), modelName);
        //ent->setCastShadows(true);
        ent->setQueryFlags(0);

        SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode(entityName.str(), location);
        node->attachObject(ent);

        // Make white models face the opposite direction.
        if (modelName.find("white") == 0)
        {
            node->yaw(Degree(180));
        }
        node->setInitialState();
    }

    void createScene()
    {
        Entity *ent;
        Light *light;

        mSceneMgr->setAmbientLight(ViewConstants::AMBIENT_COLOUR);
        //mSceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE);

        for (int i = 0; i < 8; i++)
        {
            createPiece('P', "black_pawn.mesh", Vector3(-700 + i*200, 0, -500));
        }
        createPiece('R', "black_rook.mesh", Vector3(-700, 0, -700));
        createPiece('N', "black_knight.mesh", Vector3(-500, 0, -700));
        createPiece('B', "black_bishop.mesh", Vector3(-300, 0, -700));
        createPiece('Q', "black_queen.mesh", Vector3(-100, 0, -700));
        createPiece('K', "black_king.mesh", Vector3(100, 0, -700));
        createPiece('B', "black_bishop.mesh", Vector3(300, 0, -700));
        createPiece('N', "black_knight.mesh", Vector3(500, 0, -700));
        createPiece('R', "black_rook.mesh", Vector3(700, 0, -700));

        for (int i = 0; i < 8; i++)
        {
            createPiece('P', "white_pawn.mesh", Vector3(-700 + i*200, 0, 500));
        }
        createPiece('R', "white_rook.mesh", Vector3(-700, 0, 700));
        createPiece('N', "white_knight.mesh", Vector3(-500, 0, 700));
        createPiece('B', "white_bishop.mesh", Vector3(-300, 0, 700));
        createPiece('Q', "white_queen.mesh", Vector3(-100, 0, 700));
        createPiece('K', "white_king.mesh", Vector3(100, 0, 700));
        createPiece('B', "white_bishop.mesh", Vector3(300, 0, 700));
        createPiece('N', "white_knight.mesh", Vector3(500, 0, 700));
        createPiece('R', "white_rook.mesh", Vector3(700, 0, 700));

        //ent = mSceneMgr->createEntity("BoardEntity", "board.mesh");
        //mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ent);
        //ent->setCastShadows(false);
        Plane plane(Vector3::UNIT_Y, 0);

        MeshManager::getSingleton().createPlane("square",
            ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
            200, 200, 1, 1, true, 1, 1, 1, Vector3::UNIT_Z);
        for (int j = 0; j < 8; j++)
        {
            for (int i = 0; i < 8; i++)
            {
                std::ostringstream name;
                name << "Board" << i << "," << j;

                ent = mSceneMgr->createEntity(name.str(), "square");
                mSceneMgr->getRootSceneNode()->createChildSceneNode(name.str(),
                    Vector3(-700 + i * 200, 0, -700 + j * 200)
                    )->attachObject(ent);

                if ((i + j) % 2 == 0)
                {
                    ent->setMaterialName("board/square/black");
                }
                else
                {
                    ent->setMaterialName("board/square/white");
                }
                ent->setQueryFlags(1 << 0);
            }
        }

        light = mSceneMgr->createLight("Yellow");
        light->setType(Light::LT_POINT);
        light->setPosition(Vector3(150, 250, 150));
        light->setDiffuseColour(ViewConstants::YELLOW_COLOUR);
        light->setSpecularColour(1.0, 1.0, 1.0);

        light = mSceneMgr->createLight("Blue");
        light->setType(Light::LT_POINT);
        light->setPosition(Vector3(-150, 250, -150));
        light->setDiffuseColour(ViewConstants::BLUE_COLOUR);
        light->setSpecularColour(1.0, 1.0, 1.0);

        CEGUI::WindowManager *win = CEGUI::WindowManager::getSingletonPtr();
        CEGUI::Window *sheet = win->createWindow("DefaultGUISheet", "View/Sheet");

        CEGUI::Window *quit = win->createWindow("TaharezLook/Button", "View/QuitButton");
        quit->setText("Quit");
        quit->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));

        CEGUI::Window *debug = win->createWindow("TaharezLook/Button", "View/DebugButton");
        debug->setText("Debug");
        debug->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0.05, 0)));
        debug->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));

        sheet->addChildWindow(quit);
        sheet->addChildWindow(debug);
        mSystem->setGUISheet(sheet);

        //CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();
        //CEGUI::Window *quit = wmgr->getWindow("View/QuitButton");
        quit->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&ViewFrameListener::quit, mListener));
        debug->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&ViewFrameListener::toggleDebugInfo, mListener));
    }
};
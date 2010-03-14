#include <Ogre.h>
#include <OIS/OIS.h>
#include <CEGUI/CEGUI.h>
#include <OgreCEGUIRenderer.h>

using namespace Ogre;

class BufferedInputHandler : public OIS::KeyListener, public OIS::MouseListener
{
public:
    BufferedInputHandler(RenderWindow *window, Camera *camera, SceneManager *sceneMgr)
        : mLMouseDown(false), mRMouseDown(false), mWindow(window), mCamera(camera),
        mSceneMgr(sceneMgr), mRaySceneQuery(sceneMgr->createRayQuery(Ray())),
        mSelectedObject(0)
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

        return true;
    }
    virtual bool keyReleased(const OIS::KeyEvent &arg)
    {
        CEGUI::System::getSingleton().injectKeyUp(arg.key);

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
        } // if

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
        } // if

        // Right mouse button up
        else if (id == OIS::MB_Right)
        {
            onRightReleased(arg);
            mRMouseDown = false;
        }
        return true;
    }

protected:
    bool mLMouseDown, mRMouseDown;     // True if the mouse buttons are down
    RenderWindow *mWindow;
    Camera *mCamera;
    SceneManager *mSceneMgr;           // A pointer to the scene manager
    RaySceneQuery *mRaySceneQuery;     // The ray scene query pointer
    SceneNode *mSelectedObject;         // The selected object
    //CEGUI::Renderer *mGUIRenderer;     // CEGUI renderer
    //bool mRobotMode;                   // The current state

    virtual void onLeftPressed(const OIS::MouseEvent &arg)
    {
        std::cout << "left pressed" << std::endl;
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
            std::cout << "iterating" << std::endl;
            if (itr->movable)
            {
                if (mSelectedObject)
                {
                    // TODO: find the object above this square by O(n) iterating everything
                    // Or maybe just send the request to model and update everything or something?
                    SceneNode *targetNode = itr->movable->getParentSceneNode();
                    std::cout << mSelectedObject->getName() << " -> " << targetNode->getName() << std::endl;
                    mSelectedObject->showBoundingBox(false);
                    mSelectedObject = 0;
                }
                else
                {
                    mSelectedObject = itr->movable->getParentSceneNode();
                    mSelectedObject->showBoundingBox(true);
                }
                break;
            }
        }
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
        mRenderer(0), mSystem(0), mListener(0),
        mEntityCount(0)
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

    virtual void createPiece(const std::string& modelName, const Vector3& location)
    {
        std::ostringstream entityName;
        entityName << modelName << mEntityCount;
        mEntityCount++;

        Entity* ent = mSceneMgr->createEntity(entityName.str(), modelName);
        //ent->setCastShadows(true);
        ent->setQueryFlags(0);

        mSceneMgr->getRootSceneNode()->createChildSceneNode(location)->attachObject(ent);
    }

    void createScene()
    {
        Entity *ent;
        Light *light;

        mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));
        //mSceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE);

        for (int i = 0; i < 8; i++)
        {
            createPiece("black_pawn.mesh", Vector3(-700 + i*200, 0, -500));
        }
        createPiece("black_rook.mesh", Vector3(-700, 0, -700));
        createPiece("black_knight.mesh", Vector3(-500, 0, -700));
        createPiece("black_bishop.mesh", Vector3(-300, 0, -700));
        createPiece("black_queen.mesh", Vector3(-100, 0, -700));
        createPiece("black_king.mesh", Vector3(100, 0, -700));
        createPiece("black_bishop.mesh", Vector3(300, 0, -700));
        createPiece("black_knight.mesh", Vector3(500, 0, -700));
        createPiece("black_rook.mesh", Vector3(700, 0, -700));

        for (int i = 0; i < 8; i++)
        {
            createPiece("white_pawn.mesh", Vector3(-700 + i*200, 0, 500));
        }
        createPiece("white_rook.mesh", Vector3(-700, 0, 700));
        createPiece("white_knight.mesh", Vector3(-500, 0, 700));
        createPiece("white_bishop.mesh", Vector3(-300, 0, 700));
        createPiece("white_queen.mesh", Vector3(-100, 0, 700));
        createPiece("white_king.mesh", Vector3(100, 0, 700));
        createPiece("white_bishop.mesh", Vector3(300, 0, 700));
        createPiece("white_knight.mesh", Vector3(500, 0, 700));
        createPiece("white_rook.mesh", Vector3(700, 0, 700));

        //ent = mSceneMgr->createEntity("BoardEntity", "board.mesh");
        //mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ent);
        //ent->setCastShadows(false);
        Plane plane(Vector3::UNIT_Y, 0);

        MeshManager::getSingleton().createPlane("square",
            ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
            200,200,1,1,true,1,1,1,Vector3::UNIT_Z);
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

        light = mSceneMgr->createLight("Light1");
        light->setType(Light::LT_POINT);
        light->setPosition(Vector3(150, 250, 150));
        light->setDiffuseColour(1.0, 1.0, 0.4);
        light->setSpecularColour(1.0, 1.0, 1.0);

        light = mSceneMgr->createLight("Light2");
        light->setType(Light::LT_POINT);
        light->setPosition(Vector3(-150, 250, -150));
        light->setDiffuseColour(0.4, 0.4, 1.0);
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
#include <Ogre.h>
#include <OIS/OIS.h>
#include <CEGUI/CEGUI.h>
#include <OgreCEGUIRenderer.h>

using namespace Ogre;

class BufferedInputHandler : public OIS::KeyListener, public OIS::MouseListener
{
public:
    BufferedInputHandler(OIS::Keyboard *keyboard = 0, OIS::Mouse *mouse = 0)
    {
        if (keyboard)
        {
            keyboard->setEventCallback(this);
        }

        if (mouse)
        {
            mouse->setEventCallback(this);
        }
    }

    // KeyListener
    virtual bool keyPressed(const OIS::KeyEvent &arg) { return true; }
    virtual bool keyReleased(const OIS::KeyEvent &arg) { return true; }

    // MouseListener
    virtual bool mouseMoved(const OIS::MouseEvent &arg) { return true; }
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) { return true; }
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) { return true; }
};

class ViewFrameListener : public FrameListener
{
public:
    ViewFrameListener(OIS::Keyboard *keyboard)
        : mKeyboard(keyboard)
    {
    }

    bool frameStarted(const FrameEvent& evt)
    {
        mKeyboard->capture();
        return !mKeyboard->isKeyDown(OIS::KC_ESCAPE);
    }

protected:
    OIS::Keyboard *mKeyboard;
};

class View
{
public:
    View() : mRoot(0), mKeyboard(0), mMouse(0), mInputManager(0),
        mRenderer(0), mSystem(0), mListener(0),
        entityCount(0)
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

    int entityCount;

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
        RenderWindow *win = mRoot->getAutoCreatedWindow();

        win->getCustomAttribute("WINDOW", &windowHnd);
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
        mListener = new ViewFrameListener(mKeyboard);
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
        std::stringstream entityName(modelName);
        entityName << entityCount;
        entityCount++;

        Entity* ent = mSceneMgr->createEntity(entityName.str(), modelName);
        ent->setCastShadows(true);
        mSceneMgr->getRootSceneNode()->createChildSceneNode(location)->attachObject(ent);
    }

    void createScene()
    {
        Entity *ent;
        Light *light;

        mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));
        mSceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE);

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

        ent = mSceneMgr->createEntity("BoardEntity", "board.mesh");
        mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ent);
        ent->setCastShadows(false);

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
    }
};
#include "view.h"
#include "viewframelistener.h"
#include "viewconstants.h"

View::~View()
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

void View::createRoot()
{
#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    mRoot = new Root("plugins-windows.cfg");
#else
    mRoot = new Root("plugins-linux.cfg");
#endif
}

void View::defineResources()
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

void View::setupRenderSystem()
{
    if (!mRoot->restoreConfig() && !mRoot->showConfigDialog())
        throw Exception(52, "User canceled the config dialog!", "Application::setupRenderSystem()");
}

void View::createRenderWindow()
{
    mRoot->initialise(true, "Not Another Lousy Chess Game");
}

void View::initializeResourceGroups()
{
    TextureManager::getSingleton().setDefaultNumMipmaps(5);
    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void View::setupScene()
{
    mSceneMgr = mRoot->createSceneManager(ST_GENERIC, "Default SceneManager");
    createCamera();
    mWindow = mRoot->getAutoCreatedWindow();
    createViewports();
}

void View::setupInputSystem()
{
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
    OIS::ParamList pl;

    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
    //pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
    //pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
    pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
    pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
    pl.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
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
    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = mWindow->getWidth();
    ms.height = mWindow->getHeight();
}

void View::setupCEGUI()
{
    mRenderer = new CEGUI::OgreCEGUIRenderer(mWindow, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, mSceneMgr);
    mSystem = new CEGUI::System(mRenderer);

    CEGUI::SchemeManager::getSingleton().loadScheme("TaharezLookSkin.scheme");

    mSystem->setDefaultMouseCursor("TaharezLook", "MouseArrow");
    mSystem->setDefaultFont("BlueHighway-12");

    CEGUI::MouseCursor::getSingleton().setImage(CEGUI::System::getSingleton().getDefaultMouseCursor());

    //CEGUI::MouseCursor::getSingleton().setVisible(false);
}

void View::createFrameListener()
{
    mListener = new ViewFrameListener(mKeyboard, mMouse, mWindow, mCamera, mSceneMgr);
    mRoot->addFrameListener(mListener);
}

void View::startRenderLoop()
{
    mRoot->startRendering();
}

void View::createCamera()
{
    mCamera = mSceneMgr->createCamera("PlayerCam");
    // set its position, direction
    mCamera->setPosition(Vector3(0,1500,1500));
    mCamera->lookAt(Vector3(0,0,-45.0));
    mCamera->setNearClipDistance(5);
}

void View::createViewports()
{
    // Create one viewport, entire window
    Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(ColourValue(0,0,0));
    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
}

// The function to create our decal projector
void View::createProjector()
{
    mDecalFrustum = new Frustum();
    mProjectorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("DecalProjectorNode");
    mProjectorNode->attachObject(mDecalFrustum);
    mProjectorNode->setPosition(0,5,0);
}

// A function to take an existing material and make it receive the projected decal
void View::makeMaterialReceiveDecal(const String &matName)
{
}

void View::createPiece(char type, const std::string& modelName,
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

void View::createScene()
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
#include "view.h"
#include "viewframelistener.h"
#include "viewconstants.h"
#include "../middleman.h"
#include "animationfactory.h"

View::~View()
{
    if( mInputManager )
    {
        mInputManager->destroyInputObject( mMouse );
        mInputManager->destroyInputObject( mKeyboard );
        OIS::InputManager::destroyInputSystem(mInputManager);
        mInputManager = 0;
    }

    delete mSystem;
    mSystem = 0;
    delete mRenderer;
    mRenderer = 0;

    delete mListener;
    mListener = 0;
    delete mRoot;
    mRoot = 0;
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
    mRoot->initialise(true, "NALCG");
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
    mSystem->setMultiClickTimeout(std::numeric_limits<double>::min());
    //CEGUI::MouseCursor::getSingleton().setVisible(false);
}

void View::createFrameListener()
{
    mListener = new ViewFrameListener(mKeyboard, mMouse, mWindow, mCamera, mSceneMgr, this);
    mRoot->addFrameListener(mListener);


    WindowEventUtilities::addWindowEventListener(mWindow, this);
}

void View::startRenderLoop()
{
    mRoot->startRendering();
}

//Unattach OIS before window shutdown (very important under Linux)
void View::windowClosed(RenderWindow* rw)
{
    //Only close for window that created OIS (the main window in these demos)
    if( rw == mWindow )
    {
        if( mInputManager )
        {
            mInputManager->destroyInputObject( mMouse );
            mInputManager->destroyInputObject( mKeyboard );
            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
            mListener->quit(CEGUI::EventArgs());
        }
    }
}

void View::createCamera()
{
    mCamera = mSceneMgr->createCamera("PlayerCam");
    // set its position, direction
    mCamera->setPosition(Vector3(0, 1800, 1900));
    mCamera->lookAt(Vector3(0, 0, -45.0));
    mCamera->setPosition(Vector3(0, 18000, 19000));
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
    // set up the main decal projection frustum
    mDecalFrustum = new Frustum();
    mProjectorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("DecalProjectorNode");
    mProjectorNode->attachObject(mDecalFrustum);
    mProjectorNode->setPosition(0,500,1500);
    mDecalFrustum->setQueryFlags(0);

    // include these two lines if you don't want perspective projection
    //mDecalFrustum->setProjectionType(PT_ORTHOGRAPHIC);
    //mDecalFrustum->setNearClipDistance(25);

    // set up the perpendicular filter texture frustum
    mFilterFrustum = new Frustum();
    mFilterFrustum->setProjectionType(PT_ORTHOGRAPHIC);
    SceneNode *filterNode = mProjectorNode->createChildSceneNode("DecalFilterNode");
    filterNode->attachObject(mFilterFrustum);
    filterNode->setOrientation(Quaternion(Degree(90),Vector3::UNIT_Y));
    mFilterFrustum->setQueryFlags(0);
}

// A function to take an existing material and make it receive the projected decal
void View::makeMaterialReceiveDecal(const String &matName)
{
    // get the material
    MaterialPtr mat = (MaterialPtr)MaterialManager::getSingleton().getByName(matName);

    // create a new pass in the material to render the decal
    Pass *pass = mat->getTechnique(0)->createPass();

    // set our pass to blend the decal over the model's regular texture
    pass->setSceneBlending(SBT_TRANSPARENT_ALPHA);
    pass->setDepthBias(1);

    // set the decal to be self illuminated instead of lit by scene lighting
    pass->setLightingEnabled(false);

    // set up the decal's texture unit
    TextureUnitState *texState = pass->createTextureUnitState("decal.png");
    texState->setProjectiveTexturing(true, mDecalFrustum);
    texState->setTextureAddressingMode(TextureUnitState::TAM_CLAMP);
    texState->setTextureFiltering(FO_POINT, FO_LINEAR, FO_NONE);

    // set up the filter texture's texture unit
    texState = pass->createTextureUnitState("decal_filter.png");
    texState->setProjectiveTexturing(true, mFilterFrustum);
    texState->setTextureAddressingMode(TextureUnitState::TAM_CLAMP);
    texState->setTextureFiltering(TFO_NONE);
}

void View::createPiece(char type, const std::string& modelName,
                       const Vector3& location)
{
    std::ostringstream entityName;
    entityName << type << modelName << location.x << location.y << location.z;

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
    /*else if (type == 'K')
    {
        AnimationState *mAnimationState = ent->getAnimationState("wrooktest");
        mAnimationState->setLoop(true);
        mAnimationState->setEnabled(true);
        mAnimationState->addTime(2);
    }*/
    node->setInitialState();
}

CEGUI::Window* View::createGUIComponent(const std::string& text, double x, double y,
                                        double sizeX, double sizeY, const std::string& type)
{
    CEGUI::WindowManager* win = CEGUI::WindowManager::getSingletonPtr();
    CEGUI::Window* sheet = win->getWindow("View/Sheet");

    CEGUI::Window* button = win->createWindow("TaharezLook/" + type, "View/" + text + type);
    button->setText(text);
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(x, 0), CEGUI::UDim(y, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(sizeX, 0), CEGUI::UDim(sizeY, 0)));

    sheet->addChildWindow(button);
    return button;
}

void View::createGUI()
{
    CEGUI::WindowManager* win = CEGUI::WindowManager::getSingletonPtr();
    CEGUI::Window* sheet = win->createWindow("DefaultGUISheet", "View/Sheet");
    mSystem->setGUISheet(sheet);

    createGUIComponent("Animation speed: 1x", 0, 0, 0.19, 0.08, "StaticText");

    CEGUI::Scrollbar* animationSpeedSlider = static_cast<CEGUI::Scrollbar*>(win->
        createWindow("TaharezLook/HorizontalScrollbar", "View/AnimationSpeedSlider"));
    animationSpeedSlider->setPosition(CEGUI::UVector2(CEGUI::UDim(0.005, 0), CEGUI::UDim(0.055, 0)));
    animationSpeedSlider->setSize(CEGUI::UVector2(CEGUI::UDim(0.17, 0), CEGUI::UDim(0.02, 0)));
    animationSpeedSlider->setDocumentSize(4);
    animationSpeedSlider->setScrollPosition(1);

    sheet->addChildWindow(animationSpeedSlider);
    animationSpeedSlider->subscribeEvent(
        CEGUI::Scrollbar::EventScrollPositionChanged,
        CEGUI::Event::Subscriber(&ViewFrameListener::handleAnimationSpeedChanged, mListener));

    createGUIComponent("Undo", 0, 0.10, 0.1, 0.04)->subscribeEvent(CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&View::undo, this));

    createGUIComponent("Restart", 0, 0.17, 0.1, 0.04)->subscribeEvent(CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&View::restart, this));

    createGUIComponent("Quit", 0, 0.22, 0.1, 0.04)->subscribeEvent(CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&ViewFrameListener::quit, mListener));

    createGUIComponent("FPS info", 0, 0.29, 0.1, 0.04)->subscribeEvent(CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&ViewFrameListener::toggleDebugInfo, mListener));

    createGUIComponent("Dev", 0, 0.34, 0.1, 0.04)->subscribeEvent(CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&View::dev, this));
}

void View::createScene()
{
    Light *light;

    mSceneMgr->setAmbientLight(ViewConstants::AMBIENT_COLOUR);
    //mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);

    light = mSceneMgr->createLight("Yellow");
    light->setType(Light::LT_POINT);
    light->setPosition(Vector3(1500, -25000, 1500));
    light->setDiffuseColour(ViewConstants::YELLOW_COLOUR);
    light->setSpecularColour(1.0, 1.0, 1.0);

    light = mSceneMgr->createLight("Blue");
    light->setType(Light::LT_POINT);
    light->setPosition(Vector3(-1500, 1500, -1500));
    light->setDiffuseColour(ViewConstants::BLUE_COLOUR);
    light->setSpecularColour(1.0, 1.0, 1.0);


    /*// Create the decal projector
    createProjector();

    // Make all of the materials in the head entities receive the decal
    //for (unsigned int i = 0; i < ent->getNumSubEntities(); i++)
        //makeMaterialReceiveDecal(ent->getSubEntity(i)->getMaterialName());
    makeMaterialReceiveDecal("board/square/white");
    makeMaterialReceiveDecal("board/square/black");
    makeMaterialReceiveDecal("asdfasf");
    makeMaterialReceiveDecal("color_247_242_229");
    makeMaterialReceiveDecal("22-Default");
    makeMaterialReceiveDecal("18-Default");*/
    //mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox");
    mSceneMgr->setSkyDome(true, "Sky", 10, 4);
}


void View::createInitialExplosion()
{
    SceneNode* explosion = mSceneMgr->getRootSceneNode()->createChildSceneNode(
        "Initial explosion", Vector3(0, 0, 0));
    BleedingAnimation* anim = AnimationFactory::createBleedingAnimation(
        explosion, mSceneMgr, 0, 1.8, "Effects/Explosion");
    mListener->getAnimationManager().addAnimation(anim);

}

void View::createGround()
{
    Entity* ent = mSceneMgr->createEntity("ground", "ground.mesh");
    ent->setQueryFlags(0);
    mSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(0, -50, 0))->attachObject(ent);

    /*ent = mSceneMgr->createEntity("podium", "podium.mesh");
    ent->setQueryFlags(0);
    mSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(0, -50, 0))->attachObject(ent);*/
}

void View::createBoard(const Board* board)
{
    mBoardWidth = board->getWidth();
    mBoardHeight = board->getHeight();

    using ViewConstants::SQUARE_SIDE_LENGTH;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            char symbol = board->getSymbolAt(i, j);
            if (symbol)
            {
                char upperCaseSymbol = symbol & (~(1 << 5));
                createPiece(upperCaseSymbol, getMeshName(symbol), convertPosition(i, j));
            }
        }
    }

    Plane plane(Vector3::UNIT_Y, 0);

    /*MeshManager::getSingleton().createPrefabCube().createPlane("square",
        ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
        SQUARE_SIDE_LENGTH, SQUARE_SIDE_LENGTH, 1, 1, true, 1, 1, 1, Vector3::UNIT_Z);*/
    for (int j = 0; j < int(mBoardHeight); j++)
    {
        for (int i = 0; i < int(mBoardWidth); i++)
        {
            std::ostringstream name;
            name << i << " " << j;
            Entity* ent = mSceneMgr->createEntity(name.str(), "square.mesh");

            Vector3 position = convertPosition(i, j);
            SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode(
                name.str(), position);
            node->attachObject(ent);

            if ((i + j) % 2 == 1)
            {
                ent->setMaterialName("board/square/black");
            }
            else
            {
                ent->setMaterialName("board/square/white");
            }
            ent->setQueryFlags(1 << 0);

            // Create transparent squares on top of normal squares.
            // These squares indicate possible movement locations.
            name << " s";
            ent = mSceneMgr->createEntity(name.str(), "square.mesh");
            ent->setMaterialName("board/square/move");
            ent->setQueryFlags(0);
            node->attachObject(ent);
            ent->setVisible(false);
        }
    }
}

std::string View::getMeshName(char symbol) const
{
    switch (symbol)
    {
    case 'P': return "black_pawn.mesh";
    case 'R': return "black_rook.mesh";
    case 'N': return "black_knight.mesh";
    case 'B': return "black_bishop.mesh";
    case 'Q': return "black_queen.mesh";
    case 'K': return "black_king.mesh";

    case 'p': return "white_pawn.mesh";
    case 'r': return "white_rook.mesh";
    case 'n': return "white_knight.mesh";
    case 'b': return "white_bishop.mesh";
    case 'q': return "white_queen.mesh";
    case 'k': return "white_king.mesh";
    default:
        std::ostringstream errorMessage;
        errorMessage << "getMeshName(): Unknown symbol: " << symbol;
        errorMessage << " (" << int(symbol) << ")";
        throw std::runtime_error(errorMessage.str());
    }
}

void View::convertPosition(const Vector3& position, int* x, int* y) const
{
    int sideLength = ViewConstants::SQUARE_SIDE_LENGTH;
    int offsetX = (getBoardWidth() - 1) * sideLength / 2;
    int offsetY = (getBoardHeight() - 1) * sideLength / 2;
    // +0.5 for rounding.
    *x = (position.x + offsetX + 0.5) / sideLength;
    *y = (position.z + offsetY + 0.5) / sideLength;
}

Vector3 View::convertPosition(int x, int y) const
{
    int sideLength = ViewConstants::SQUARE_SIDE_LENGTH;
    int offsetX = (getBoardWidth() - 1) * sideLength / 2;
    int offsetY = (getBoardHeight() - 1) * sideLength / 2;

    return Vector3(x * sideLength - offsetX, 0, y * sideLength - offsetY);
}

bool View::undo(const CEGUI::EventArgs& e)
{
    mMiddleman->undo();
    return true;
}

bool View::restart(const CEGUI::EventArgs& e)
{
    mMiddleman->undo(mRound);
    return true;
}

bool View::dev(const CEGUI::EventArgs& e)
{
    mMiddleman->undo(Middleman::HALF_TURN);
    return true;
}

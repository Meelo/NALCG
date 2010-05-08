#include "view.h"

#include "viewframelistener.h"
#include "viewconstants.h"
#include "../middleman.h"
#include "animationfactory.h"
#include "bleedinganimation.h"
#include "../logic/chessboard.h"

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
    if (!mWindow->isFullScreen())
    {
        pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
        pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));

        pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
        pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));

        pl.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
    }
    mInputManager = OIS::InputManager::createInputSystem(pl);

    try
    {
        mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
        mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));
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
    CEGUI::MouseCursor::getSingleton().setVisible(false);
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

    mCamera->setPosition(ViewConstants::WHITE_CAMERA_POSITION * 10);
    mCamera->lookAt(Vector3::ZERO);
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

Entity* View::loadEntity(const std::string& entityName, const std::string& modelName)
{
    Entity* ent;
    if (mSceneMgr->hasEntity(entityName))
    {
        ent = mSceneMgr->getEntity(entityName);
    }
    else
    {
        ent = mSceneMgr->createEntity(entityName, modelName);
    }

    ent->setQueryFlags(0);

    AnimationStateSet* animations = ent->getAllAnimationStates();
    if (animations)
    {
        AnimationStateIterator it = animations->getAnimationStateIterator();
        while (it.hasMoreElements())
        {
            AnimationState* animationState = it.getNext();
            animationState->setEnabled(true);
            animationState->setLoop(true);
            animationState->setTimePosition(0);
        }
    }
    return ent;
}

SceneNode* View::createPiece(char type, const std::string& modelName,
                       const Vector3& location, SceneNode* parent)
{
    if (!parent)
    {
        parent = mSceneMgr->getRootSceneNode();
    }

    std::ostringstream entityName;
    entityName << type << modelName << location.x << location.y << location.z << parent->getName();

    SceneNode* node = parent->createChildSceneNode(entityName.str(), location);

    Entity* ent;
    if (mSceneMgr->hasEntity(entityName.str()))
    {
        ent = mSceneMgr->getEntity(entityName.str());
    }
    else
    {
        ent = mSceneMgr->createEntity(entityName.str(), modelName + ".mesh");
    }
    //ent->setCastShadows(true);
    ent->setQueryFlags(0);
    node->attachObject(ent);

    if (type == 'P' || type == 'R')
    {
        node->attachObject(loadEntity(entityName.str() + "l", modelName + "_left_leg.mesh"));
        node->attachObject(loadEntity(entityName.str() + "r", modelName + "_right_leg.mesh"));
        if (type == 'R')
        {
            node->attachObject(loadEntity(entityName.str() + "a", modelName + "_right_arm.mesh"));
            node->attachObject(loadEntity(entityName.str() + "wb", modelName + "_weapon_base.mesh"));
            node->attachObject(loadEntity(entityName.str() + "w", modelName + "_weapon.mesh"));
        }
    }

    // Make white models face the opposite direction.
    if (modelName.find("white") == 0)
    {
        node->yaw(Degree(180));
    }
    node->setInitialState();
    return node;
}

CEGUI::Window* View::createGUIComponent(const std::string& text, double x, double y,
                                        double sizeX, double sizeY, const std::string& type, bool setText, bool visible)
{
    CEGUI::WindowManager* win = CEGUI::WindowManager::getSingletonPtr();
    CEGUI::Window* sheet = win->getWindow("View/Sheet");

    CEGUI::Window* button = win->createWindow("TaharezLook/" + type, "View/" + text + type);
    if (setText)
    {
        button->setText(text);
    }
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(x, 0), CEGUI::UDim(y, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(sizeX, 0), CEGUI::UDim(sizeY, 0)));

    sheet->addChildWindow(button);
    button->setVisible(visible);
    return button;
}

void View::createGUI()
{
    CEGUI::WindowManager* win = CEGUI::WindowManager::getSingletonPtr();
    CEGUI::Window* sheet = win->createWindow("DefaultGUISheet", "View/Sheet");
    mSystem->setGUISheet(sheet);

    createGUIComponent("Animation speed", 0.0, 0.165, 0.19, 0.05, "StaticText", false, false)
        ->setText("Animation speed: 1x");

    CEGUI::Scrollbar* animationSpeedSlider = static_cast<CEGUI::Scrollbar*>(
        createGUIComponent("Animation speed", 0.0, 0.215, 0.19, 0.02, "HorizontalScrollbar", false, false));
    animationSpeedSlider->setDocumentSize(4);
    animationSpeedSlider->setScrollPosition(1);
    animationSpeedSlider->subscribeEvent(
        CEGUI::Scrollbar::EventScrollPositionChanged,
        CEGUI::Event::Subscriber(&ViewFrameListener::handleAnimationSpeedChanged, mListener));

    createGUIComponent("Undo", 0.875, 0.45, 0.12, 0.04)->subscribeEvent(CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&View::undo, this));

    createGUIComponent("Restart", 0.875, 0.505, 0.12, 0.04)->subscribeEvent(CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&View::restart, this));

    createGUIComponent("Quit", 0.875, 0.55, 0.12, 0.04)->subscribeEvent(CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&ViewFrameListener::quit, mListener));

    createGUIComponent("FPS info", 0.875, 0.6, 0.12, 0.04, "Button", true,
        false)->subscribeEvent(CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&ViewFrameListener::toggleDebugInfo, mListener));

    createGUIComponent("Game log", 0.875, 0.005, 0.12, 0.05, "StaticText");
    createGUIComponent("Log", 0.875, 0.05, 0.12, 0.4, "Listbox")
        ->subscribeEvent(CEGUI::Listbox::EventSelectionChanged,
        CEGUI::Event::Subscriber(&View::visitSelectedLog, this));

    CEGUI::Window* chooseButton = createGUIComponent("Choose queen", 0.4, 0.30, 0.2, 0.1);
    chooseButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&View::chooseQueen, this));
    chooseButton->setVisible(false);

    chooseButton = createGUIComponent("Choose rook", 0.4, 0.40, 0.2, 0.1);
    chooseButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&View::chooseRook, this));
    chooseButton->setVisible(false);

    chooseButton = createGUIComponent("Choose knight", 0.4, 0.50, 0.2, 0.1);
    chooseButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&View::chooseKnight, this));
    chooseButton->setVisible(false);

    chooseButton = createGUIComponent("Choose bishop", 0.4, 0.60, 0.2, 0.1);
    chooseButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&View::chooseBishop, this));
    chooseButton->setVisible(false);

    createGUIComponent("Move assistance level", 0.00, 0.09, 0.18, 0.05, "StaticText", true, false);

    CEGUI::Spinner* moveAssistanceSpinner = static_cast<CEGUI::Spinner*>(
        createGUIComponent("Move assistance", 0.18, 0.09, 0.05, 0.05, "Spinner", false, false));
    moveAssistanceSpinner->setCurrentValue(3.0);
    moveAssistanceSpinner->setMinimumValue(0.0);
    moveAssistanceSpinner->setMaximumValue(3.0);
    moveAssistanceSpinner->subscribeEvent(
        CEGUI::Spinner::EventValueChanged,
        CEGUI::Event::Subscriber(&BufferedInputHandler::handleMoveAssistanceChanged,
        mListener->getHandler()));

    CEGUI::Window* unsafe = createGUIComponent("Unsafe mode", 0.25, 0.045, 0.3, 0.04, "Checkbox", true, false);
    unsafe->setText("Allow concurrent animations (UNSAFE!)");
    unsafe->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged,
        CEGUI::Event::Subscriber(&BufferedInputHandler::handleSafeModeChanged, mListener->getHandler()));

    createGUIComponent(ViewConstants::SHOW_ADDITIONAL, 0.005, 0.005, 0.22, 0.05)->subscribeEvent(CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&ViewFrameListener::hideGUI, mListener));
    
    // White dropdown menus
    createGUIComponent("White:", 0.3, 0.0, 0.15, 0.04, "StaticText", true, true);

    CEGUI::Combobox* white = static_cast<CEGUI::Combobox*>(createGUIComponent(
        "White", 0.3, 0.045, 0.15, 0.2, "Combobox", false, true));
    white->setReadOnly(true);
    white->setText("Human");
    white->addItem(new CEGUI::ListboxTextItem("Human"));
    white->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted,
        CEGUI::Event::Subscriber(&View::updateControllers, this));
    
    // Swap sides button
    createGUIComponent("<=>", 0.475, 0.02, 0.04, 0.05)->subscribeEvent(CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&View::swapPlayers, this));

    // Black dropdown menus
    createGUIComponent("Black:", 0.54, 0.0, 0.15, 0.04, "StaticText", true, true);

    CEGUI::Combobox* black = static_cast<CEGUI::Combobox*>(createGUIComponent(
        "Black", 0.54, 0.045, 0.15, 0.2, "Combobox", false, true));
    black->setReadOnly(true);
    black->setText("Human");
    black->addItem(new CEGUI::ListboxTextItem("Human"));
    black->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted,
        CEGUI::Event::Subscriber(&View::updateControllers, this));

    for (std::size_t i = 0; i < mMiddleman->getAICount(); i++)
    {
        std::string name = mMiddleman->getAIInfoAt(i).getName();
        white->addItem(new CEGUI::ListboxTextItem(name));
        black->addItem(new CEGUI::ListboxTextItem(name));
    }

}

void View::recreateLog()
{
    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window* logWindow = wmgr.getWindow("View/LogListbox");
    CEGUI::Listbox* logList = static_cast<CEGUI::Listbox*>(logWindow);
    logList->resetList();

    const std::vector<std::string>& gameLog = mMiddleman->getGameLog();

    for (std::size_t i = 0; i < gameLog.size(); i++)
    {
        std::ostringstream text;
        text << (i + 1) << ". " << gameLog.at(i);
        CEGUI::ListboxTextItem* item = new CEGUI::ListboxTextItem(text.str());
        logList->addItem(item);
    }
    logList->ensureItemIsVisible(logList->getItemCount() - 1);
}

void View::recreateDeadPieces()
{
    // TODO: Check why this crashes on undo during an animation.
    const Board* board = mMiddleman->getGameStateAt(mMiddleman->getGameLog().size());
    const std::vector<Piece*> deadPieces = board->getDeadPieces();
    SceneNode* whiteDeads = mSceneMgr->getSceneNode("White dead pieces");
    SceneNode* blackDeads = mSceneMgr->getSceneNode("Black dead pieces");
    blackDeads->removeAndDestroyAllChildren();
    whiteDeads->removeAndDestroyAllChildren();

    for (std::vector<Piece*>::const_iterator it = deadPieces.begin();
        it != deadPieces.end(); it++)
    {
        char symbol = (*it)->getSymbol();
        char upperCaseSymbol = symbol & (~(1 << 5));
        SceneNode* parent = (*it)->getColour() == BLACK ? blackDeads : whiteDeads;

        int nChildren = parent->numChildren();
        int xPosition, yPosition;
        if (nChildren > 3)
        {
            xPosition = (nChildren + 1) % 3;
            yPosition = (nChildren + 1) / 3;
        }
        else
        {
            xPosition = nChildren % 2;
            yPosition = nChildren / 2;
        }
        createPiece(upperCaseSymbol, getMeshName(symbol),
            convertPosition(xPosition, yPosition * 1.5), parent)->pitch(Degree(-90));
    }
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

    // Loading the sky material so it doesn't have to be loaded while
    // playing the opening animation sequence.
    mSceneMgr->setSkyDome(true, "Sky");
    mSceneMgr->setSkyDome(false, "Sky");

    mSceneMgr->createParticleSystem("Selection", "Effects/Selection");
}


void View::createInitialExplosion()
{
    SceneNode* explosion = mSceneMgr->getRootSceneNode()->createChildSceneNode(
        "Initial explosion", Vector3(0, 0, 0));
    BleedingAnimation* anim = AnimationFactory::createBleedingAnimation(
        explosion, mSceneMgr, 0, 1.8, "Effects/Explosion");
    mListener->getAnimationManager().addAnimation(anim);

}

void View::createGround(bool visible)
{
    SceneNode* ground = mSceneMgr->getRootSceneNode()->createChildSceneNode("ground", Vector3(0, -50, 0));
    SceneNode* water = ground->createChildSceneNode("water");

    Entity* ent = mSceneMgr->createEntity("ground", "ground.mesh");
    ent->setQueryFlags(0);
    ground->attachObject(ent);

    ent = mSceneMgr->createEntity("water", "water.mesh");
    ent->setQueryFlags(0);
    water->attachObject(ent);

    ground->setVisible(visible);
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
            SceneNode* indicatorNode = node->createChildSceneNode(Vector3(0, 1, 0));
            indicatorNode->attachObject(ent);
            indicatorNode->scale(1.01, 1.0, 1.01);
            ent->setVisible(false);
        }
    }
    SceneNode* invalidMoveNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("InvalidMove");
    Entity* ent = mSceneMgr->createEntity("InvalidMove", "invalid_move.mesh");
    ent->setQueryFlags(0);
    invalidMoveNode->attachObject(ent);
    invalidMoveNode->setVisible(false);

    SceneNode* selectionNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Selection");
    selectionNode->attachObject(mSceneMgr->getParticleSystem("Selection"));
    selectionNode->setVisible(false);

    mSceneMgr->getRootSceneNode()->createChildSceneNode("Black dead pieces",
        Vector3(ViewConstants::SQUARE_SIDE_LENGTH * 9, -40,
        ViewConstants::SQUARE_SIDE_LENGTH * 0.5));
    mSceneMgr->getRootSceneNode()->createChildSceneNode("White dead pieces",
        Vector3(-ViewConstants::SQUARE_SIDE_LENGTH * 9, -40,
        ViewConstants::SQUARE_SIDE_LENGTH * 0.5))->yaw(Degree(180));

    mListener->setCanShowSelectablePieces(true);
    mListener->clearSelectedObject();
}

std::string View::getMeshName(char symbol) const
{
    switch (symbol)
    {
    case 'P': return "black_pawn";
    case 'R': return "black_rook";
    case 'N': return "black_knight";
    case 'B': return "black_bishop";
    case 'Q': return "black_queen";
    case 'K': return "black_king";

    case 'p': return "white_pawn";
    case 'r': return "white_rook";
    case 'n': return "white_knight";
    case 'b': return "white_bishop";
    case 'q': return "white_queen";
    case 'k': return "white_king";
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

Vector3 View::convertPosition(double x, double y) const
{
    int sideLength = ViewConstants::SQUARE_SIDE_LENGTH;
    int offsetX = (getBoardWidth() - 1) * sideLength / 2;
    int offsetY = (getBoardHeight() - 1) * sideLength / 2;

    return Vector3(x * sideLength - offsetX, 0, y * sideLength - offsetY);
}

bool View::undo(const CEGUI::EventArgs& e)
{
    if (mPast)
    {
        mMiddleman->undo(mMiddleman->getGameLog().size() - mRound);
    }
    else
    {
        mMiddleman->undo();
    }
    ensureLatestState();
    return true;
}

bool View::restart(const CEGUI::EventArgs& e)
{
    ensureLatestState();
    mMiddleman->undo(mRound);
    return true;
}

bool View::visitSelectedLog(const CEGUI::EventArgs& e)
{
    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window* logWindow = wmgr.getWindow("View/LogListbox");
    CEGUI::Listbox* logList = static_cast<CEGUI::Listbox*>(logWindow);

    CEGUI::ListboxItem* selected = logList->getFirstSelectedItem();
    if (selected)
    {

        std::size_t selectedIndex = logList->getItemIndex(selected);
        if (selectedIndex + 1 != mMiddleman->getGameLog().size())
        {
            setBoard(mMiddleman->getGameStateAt(selectedIndex + 1), selectedIndex + 1);

            mSceneMgr->setAmbientLight(ColourValue(0.43, 0.25, 0.07));
            mSceneMgr->getLight("Yellow")->setDiffuseColour(0.86, 0.5, 0.14);
            mSceneMgr->getLight("Blue")->setDiffuseColour(0.86, 0.5, 0.14);

            wmgr.getWindow("View/UndoButton")->setText("Undo future");
            mPast = true;
        }
        else
        {
            ensureLatestState();
        }
        CEGUI::ListboxItem* newSelected = logList->getListboxItemFromIndex(selectedIndex);
        if (*newSelected->getText().rbegin() != '<')
        {
            newSelected->setText(newSelected->getText() + " <");
        }
    }
    return true;
}

void View::setPromotionMove(int fromX, int fromY, int toX, int toY)
{
    // TODO: change to actual move class.
    promotionMove.clear();
    promotionMove.push_back(fromX);
    promotionMove.push_back(fromY);
    promotionMove.push_back(toX);
    promotionMove.push_back(toY);
}

bool View::chooseQueen(const CEGUI::EventArgs& e)
{
    sendPromotionMove(ChessBoard::PROMOTE_TO_QUEEN);
    return true;
}

bool View::chooseRook(const CEGUI::EventArgs& e)
{
    sendPromotionMove(ChessBoard::PROMOTE_TO_ROOK);
    return true;
}

bool View::chooseKnight(const CEGUI::EventArgs& e)
{
    sendPromotionMove(ChessBoard::PROMOTE_TO_KNIGHT);
    return true;
}

bool View::chooseBishop(const CEGUI::EventArgs& e)
{
    sendPromotionMove(ChessBoard::PROMOTE_TO_BISHOP);
    return true;
}

void View::sendPromotionMove(unsigned int promoteTo)
{
    mMiddleman->move(promotionMove.at(0), promotionMove.at(1),
        promotionMove.at(2), promotionMove.at(3),
        promoteTo);

    setChooseButtonsVisibility(false);
}

void View::setChooseButtonsVisibility(bool visible)
{
    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    wmgr.getWindow("View/Choose queenButton")->setVisible(visible);
    wmgr.getWindow("View/Choose rookButton")->setVisible(visible);
    wmgr.getWindow("View/Choose knightButton")->setVisible(visible);
    wmgr.getWindow("View/Choose bishopButton")->setVisible(visible);
}

void View::ensureLatestState()
{
    if (mPast)
    {
        mPast = false;
        CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
        wmgr.getWindow("View/UndoButton")->setText("Undo");

        std::size_t latestRound = mMiddleman->getGameLog().size();
        setBoard(mMiddleman->getGameStateAt(latestRound), latestRound);

        mSceneMgr->setAmbientLight(ViewConstants::AMBIENT_COLOUR);
        mSceneMgr->getLight("Yellow")->setDiffuseColour(ViewConstants::YELLOW_COLOUR);
        mSceneMgr->getLight("Blue")->setDiffuseColour(ViewConstants::BLUE_COLOUR);
    }
}

void View::start()
{
    try {
        createRoot();
        defineResources();
        setupRenderSystem();
        createRenderWindow();
        initializeResourceGroups();
        setupScene();
        setupInputSystem();
        setupCEGUI();
        createFrameListener();
        createGUI();
        createScene();
        createBoard(mMiddleman->getGameStateAt(0));
        createGround(false);
        createInitialExplosion();
        startRenderLoop();
    } catch( Exception& e ) {
        fprintf(stderr, "An exception has occurred: %s\n", e.what());
    }
}


bool View::updateControllers(const CEGUI::EventArgs& e)
{
    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Combobox* white = static_cast<CEGUI::Combobox*>(wmgr.getWindow("View/WhiteCombobox"));
    CEGUI::Combobox* black = static_cast<CEGUI::Combobox*>(wmgr.getWindow("View/BlackCombobox"));
    
    std::size_t whiteIndex = white->getItemIndex(white->findItemWithText(white->getText(), 0));
    std::size_t blackIndex = black->getItemIndex(black->findItemWithText(black->getText(), 0));

    mMiddleman->setControl(whiteIndex, blackIndex);
    return true;
}

bool View::swapPlayers(const CEGUI::EventArgs& e)
{
    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window* white = wmgr.getWindow("View/WhiteCombobox");
    CEGUI::Window* black = wmgr.getWindow("View/BlackCombobox");

    const CEGUI::String whitePlayer = white->getText();
    white->setText(black->getText());
    black->setText(whitePlayer);

    return updateControllers(e);
}

void View::promptChallenge(const std::string& challengerName)
{
    mMiddleman->respondToChallenge(true);
}


void View::updateUsers(const std::vector<std::string>& users)
{
    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window* logWindow = wmgr.getWindow("View/LogListbox");
    CEGUI::Listbox* logList = static_cast<CEGUI::Listbox*>(logWindow);

    // Black dropdown menus
    createGUIComponent("Black:", 0.54, 0.0, 0.15, 0.04, "StaticText", true, true);

    CEGUI::Combobox* black = static_cast<CEGUI::Combobox*>(createGUIComponent(
        "Black", 0.54, 0.045, 0.15, 0.2, "Combobox", false, true));
    black->setReadOnly(true);
    black->setText("Human");
    black->addItem(new CEGUI::ListboxTextItem("Human"));
    black->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted,
        CEGUI::Event::Subscriber(&View::updateControllers, this));
}
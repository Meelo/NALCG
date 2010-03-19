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
    MovementAnimation(const Vector3& destination, SceneNode *movingNode,
        SceneNode *targetPiece, SceneManager *sceneMgr)
        : mDestination(destination), mMovingNode(movingNode),
        mTargetPiece(targetPiece), mTargetPieceName(targetPiece->getName()),
        mSceneMgr(sceneMgr)
    {
    }

    virtual ~MovementAnimation()
    {
        // Make sure another shorter animation hasn't already destroyed the piece.
        // Using target piece name since target piece pointer might already be freed
        // and isn't safe to call.
        if (mSceneMgr->hasSceneNode(mTargetPieceName))
        {
            mSceneMgr->getRootSceneNode()->removeAndDestroyChild(mTargetPieceName);
        }
    }

    virtual bool animate(const Real& timeSinceLastFrame) = 0;

protected:
    Vector3 mDestination;
    SceneNode *mMovingNode;
    SceneNode *mTargetPiece;
    const std::string mTargetPieceName;
    SceneManager *mSceneMgr;
};

class BishopMovementAnimation : public MovementAnimation
{
public:
    BishopMovementAnimation(const Vector3& destination, SceneNode *movingNode,
        SceneNode *targetPiece, SceneManager *sceneMgr)
        : MovementAnimation(destination, movingNode, targetPiece, sceneMgr)
    {
    }

    virtual bool animate(const Real& timeSinceLastFrame)
    {
        Real distanceMoved = MOVEMENT_SPEED * timeSinceLastFrame;
        Vector3 path = mDestination - mMovingNode->getPosition();

        if (path.length() > distanceMoved)
        {
            // Normalising the vector so the speed remains constant.
            path.normalise();
            mMovingNode->translate(path * distanceMoved);

            Vector3 src = mMovingNode->getOrientation() * Vector3::UNIT_Z;
            mMovingNode->rotate(src.getRotationTo(path));
            return true; // Animation still running.
        }

        mMovingNode->setPosition(mDestination);
        mMovingNode->setOrientation(mMovingNode->getInitialOrientation());
        return false; // Animation finished.
    }
protected:
    static const int MOVEMENT_SPEED = 500;
};

class View;
class QueenMovementAnimation : public MovementAnimation
{
public:
    QueenMovementAnimation(const Vector3& destination, SceneNode *movingNode,
        SceneNode *targetPiece, SceneManager *sceneMgr)
        : MovementAnimation(destination, movingNode, targetPiece, sceneMgr),
        mAttackCount(targetPiece ? ATTACK_COUNT : 0), mPhase(1), mTrail(0), mAttackCooldown(0)
    {
        assert(sceneMgr != 0);
        createBlasts();
    }

    virtual ~QueenMovementAnimation()
    {
        // FIXME: this destruction method is too excessive.
        // Causes the game to crash if this animation ends while others are playing.
        mSceneMgr->destroyAllAnimations();
        mSceneMgr->destroyAllAnimationStates();
        mSceneMgr->destroyAllBillboardChains();
        mSceneMgr->destroyAllBillboardSets();
        mSceneMgr->destroyAllRibbonTrails();

        mMovingNode->removeAndDestroyAllChildren();

        for (std::size_t i = 0; i < mLights.size(); i++)
        {
            mSceneMgr->destroyLight(mLights.at(i)->getName());
        }

        restoreLights();
    }

    virtual bool animate(const Real& timeSinceLastFrame)
    {
        Real distanceMoved = MOVEMENT_SPEED * timeSinceLastFrame;
        Vector3 path = mDestination - mMovingNode->getPosition();
        if (path.length() > distanceMoved)
        {

            bool flyHigher = mMovingNode->getPosition().y < 500;
            switch (mPhase)
            {
            case 1:
                // Fly higher in steep angle until flying altitude is reached.
                if (mMovingNode->getPosition().y > FLYING_ALTITUDE)
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
                    if (mAttackCount > 0)
                    {
                        dimLights();
                    }
                }
                path += Vector3(0, FLYING_ALTITUDE, 0);
                break;
            case 3:
                {
                    if (mAttackCount <= 0)
                    {
                        mPhase = 4;
                    }
                    else
                    {
                        if (mAttackCooldown <= 0)
                        {
                            SceneNode* animNode = mMovingNode->createChildSceneNode();

                            Animation* anim = mSceneMgr->createAnimation(nextName(), 10);
                            anim->setInterpolationMode(Animation::IM_SPLINE);
                            //mAnimations.push_back(anim);

                            NodeAnimationTrack* track = anim->createNodeTrack(1, animNode);
                            TransformKeyFrame* kf = track->createNodeKeyFrame(0);
                            
                            int waypoint = mAttackCount % 2 == 0 ? 100 : -100;
                            int waypoint2 = mAttackCount % 4 <= 1 ? 100 : -100;
                            kf = track->createNodeKeyFrame(0.5);
                            kf->setTranslate(Vector3(waypoint, 0, waypoint2));
                            kf = track->createNodeKeyFrame(1);
                            kf->setTranslate(Vector3(0, -200, 0));
                            kf = track->createNodeKeyFrame(1.5);
                            kf->setTranslate(Vector3(0, -600, 0));

                            AnimationState* animState = mSceneMgr->createAnimationState(anim->getName());
                            animState->setEnabled(true);
                            animState->setLoop(false);
                            mAnimStateList.push_back(animState);

                            int nAttack = ATTACK_COUNT - mAttackCount;
                            mTrail->setInitialColour(nAttack, 0.3, 0.5, 1.0);
                            mTrail->setColourChange(nAttack, 0.5, 0.5, 0.5, 0.5);
                            mTrail->setInitialWidth(nAttack, 10);
                            mTrail->addNode(animNode);

                            // Add light
                            Light* l2 = mSceneMgr->createLight(nextName());
                            l2->setDiffuseColour(mTrail->getInitialColour(0));
                            animNode->attachObject(l2);
                            mLights.push_back(l2);

                            // Add billboard
                            BillboardSet* bbs = mSceneMgr->createBillboardSet(nextName(), 20);
                            bbs->createBillboard(Vector3::ZERO, mTrail->getInitialColour(0));
                            bbs->setMaterialName("Examples/Flare");
                            bbs->setQueryFlags(0);
                            animNode->attachObject(bbs);

                            mAttackCooldown = ATTACK_COOLDOWN;
                            mAttackCount--;
                        }
                        std::vector<AnimationState*>::iterator animi;
                        for (animi = mAnimStateList.begin(); animi != mAnimStateList.end(); ++animi)
                        {
                            (*animi)->addTime(timeSinceLastFrame);
                        }
                        path += Vector3(0, FLYING_ALTITUDE, 0);
                        mAttackCooldown -= timeSinceLastFrame;
                    }
                    break;
                }
            case 4:
                break;
            }

            // Normalising the vector so the speed remains constant.
            path.normalise();
            mMovingNode->translate(path * distanceMoved);

            /*if (flyHigher)
            {
            movingNode->setOrientation(movingNode->getInitialOrientation());
            Vector3 src = movingNode->getOrientation() * Vector3::UNIT_Z;
            movingNode->rotate(src.getRotationTo(path));
            }*/

            return true; // Animation still running.
        }

        mMovingNode->setPosition(mDestination);
        mMovingNode->setOrientation(mMovingNode->getInitialOrientation());
        return false; // Animation finished.
    }

    virtual void createBlasts()
    {
        NameValuePairList pairList;
        pairList["numberOfChains"] = StringConverter::toString(mAttackCount);
        pairList["maxElements"] = "80";

        mTrail = static_cast<RibbonTrail*>(
            mSceneMgr->createMovableObject(nextName(), "RibbonTrail", &pairList));
        mTrail->setMaterialName("Examples/LightRibbonTrail");
        mTrail->setTrailLength(400);
        mTrail->setQueryFlags(0);

        mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(mTrail);
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

    // TODO: Move this to more global place if others need it too.
    std::string nextName()
    {
        std::ostringstream name;
        name << "NALCG" << id++;
        return name.str();
    }

protected:
    static const int MOVEMENT_SPEED = 500;
    static const int FLYING_ALTITUDE = 500;
    static const int ATTACK_COUNT = 300;
    static const Real ATTACK_COOLDOWN;
    static int id;

    int mAttackCount;
    int mPhase;
    std::vector<AnimationState*> mAnimStateList;
    RibbonTrail* mTrail;
    Real mAttackCooldown;
    std::vector<Light*> mLights;

};

int QueenMovementAnimation::id = 1;
const Real QueenMovementAnimation::ATTACK_COOLDOWN = 0.001;

class MovementAnimationFactory
{
public:
    static MovementAnimation* createAnimation(const char type,
        const Vector3& destination, SceneNode *movingNode,
        SceneNode *targetPiece, SceneManager *sceneMgr)
    {
        switch (type)
        {
        case 'B':
            return new BishopMovementAnimation(destination, movingNode, targetPiece, sceneMgr);
        case 'Q':
            return new QueenMovementAnimation(destination, movingNode, targetPiece, sceneMgr);
        default:
            // TODO: change this to return 0 for testing when everything should be done.
            return new BishopMovementAnimation(destination, movingNode, targetPiece, sceneMgr);
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
            CEGUI::System::getSingleton().injectMousePosition(arg.state.X.abs, arg.state.Y.abs);
            //CEGUI::System::getSingleton().injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
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
            if (!it->second->animate(timeSinceLastFrame))
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

        Ray mouseRay = mCamera->getCameraToViewportRay(mousePos.d_x/arg.state.width, mousePos.d_y/arg.state.height);

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

                        mMovementAnimations[pieceNode] = 
                            MovementAnimationFactory::createAnimation(
                            *pieceNode->getName().begin(), targetNode->getPosition(),
                            pieceNode, targetPiece, mSceneMgr);
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
        //pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
        //pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
        pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
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

    void setupCEGUI()
    {
        mRenderer = new CEGUI::OgreCEGUIRenderer(mWindow, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, mSceneMgr);
        mSystem = new CEGUI::System(mRenderer);

        CEGUI::SchemeManager::getSingleton().loadScheme("TaharezLookSkin.scheme");

        mSystem->setDefaultMouseCursor("TaharezLook", "MouseArrow");
        mSystem->setDefaultFont("BlueHighway-12");

        CEGUI::MouseCursor::getSingleton().setImage(CEGUI::System::getSingleton().getDefaultMouseCursor());

        //CEGUI::MouseCursor::getSingleton().setVisible(false);
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
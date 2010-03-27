#pragma once

#include <Ogre.h>
#include <CEGUI/CEGUI.h>
#include <OIS/OIS.h>
#include <OgreCEGUIRenderer.h>
#include "../enduser.h"

using namespace Ogre;

class ViewFrameListener;
class View : public WindowEventListener, public EndUser
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

    // TODO: implement
    virtual void move(int fromX, int fromY, int toX, int toY) { }
    virtual void setBoard(const Board* const board, unsigned int round) { }
    virtual void setControl(bool white, bool black) { }
    virtual ~View();

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
    Frustum *mDecalFrustum;
    Frustum *mFilterFrustum;
    SceneNode *mProjectorNode;

    int mEntityCount;

    void createRoot();
    void defineResources();
    void setupRenderSystem();
    void createRenderWindow();
    void initializeResourceGroups();
    void setupScene();
    void setupInputSystem();
    void setupCEGUI();
    void createFrameListener();
    void startRenderLoop();
    virtual void windowClosed(RenderWindow* rw);
    virtual void createCamera();
    virtual void createViewports();

    // The function to create our decal projector
    void createProjector();

    // A function to take an existing material and make it receive the projected decal
    void makeMaterialReceiveDecal(const String &matName);

    virtual void createPiece(char type, const std::string& modelName, const Vector3& location);
    void createScene();
};

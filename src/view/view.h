#ifndef _NALCG_VIEW_H_
#define _NALCG_VIEW_H_

#include <Ogre.h>
#include <CEGUI/CEGUI.h>
#include <OIS/OIS.h>
#include <OgreCEGUIRenderer.h>
#include "../enduser.h"
#include "viewframelistener.h"

using namespace Ogre;

class View : public WindowEventListener, public EndUser
{
public:
    View() : mRoot(0), mKeyboard(0), mMouse(0), mInputManager(0),
        mRenderer(0), mSystem(0), mListener(0), mDecalFrustum(0),
        mFilterFrustum(0), mProjectorNode(0), mBoardWidth(0),
        mBoardHeight(0), mRound(0)
    {
    }

    // TODO: implement
    virtual void init(const Board* board, Middleman *middleman) {
        EndUser::init(board, middleman);
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
            createGround();
            createBoard(board);
            startRenderLoop();
        } catch( Exception& e ) {
            fprintf(stderr, "An exception has occurred: %s\n", e.what());
        } catch (std::exception& e) {
            std::cerr << "An exception has occured: " << e.what() << std::endl;
        }
    }

    virtual void setBoard(const Board* const board, unsigned int round)
    {
        mListener->getAnimationManager().finishAnimations();
        mSceneMgr->getRootSceneNode()->removeAndDestroyAllChildren();
        mSceneMgr->destroyAllEntities();
        createGround();
        createBoard(board);
        mRound = round;
    }
    virtual void move(int fromX, int fromY, int toX, int toY)
    {
        mListener->move(fromX, fromY, toX, toY);
        mRound++;
    }
    virtual void setControl(bool white, bool black) { }

    virtual int getBoardWidth() const { return mBoardWidth; }
    virtual int getBoardHeight() const { return mBoardHeight; }

    virtual void convertPosition(const Vector3& position, int* x, int* y) const;
    virtual Vector3 convertPosition(int x, int y) const;
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
    std::size_t mBoardWidth;
    std::size_t mBoardHeight;
    unsigned int mRound;

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
    void createGround();
    void createGUI();
    void createBoard(const Board* board);
    std::string getMeshName(char symbol) const;
    CEGUI::Window* createGUIComponent(const std::string& text, double x, double y,
        double sizeX, double sizeY, const std::string& type = "Button");
    bool undo(const CEGUI::EventArgs& e);
    bool restart(const CEGUI::EventArgs& e);
    bool dev(const CEGUI::EventArgs& e);

};

#endif // _NALCG_VIEW_H_

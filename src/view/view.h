#ifndef _NALCG_VIEW_H_
#define _NALCG_VIEW_H_

#include "../enduser.h"
#include "viewframelistener.h"

#include <Ogre.h>
#include <CEGUI/CEGUI.h>
#include <OIS/OIS.h>
#include <OgreCEGUIRenderer.h>

using namespace Ogre;

class View : public WindowEventListener, public EndUser
{
public:
    View() : mRoot(0), mKeyboard(0), mMouse(0), mInputManager(0),
        mRenderer(0), mSystem(0), mListener(0), mBoardWidth(0),
        mBoardHeight(0), mRound(0), mPast(false)
    {
    }

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
            createBoard(board);
            createGround(false);
            createInitialExplosion();
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
        createGround(true);
        createBoard(board);
        mRound = round;
        recreateLog();
        recreateDeadPieces();
    }
    virtual void move(int fromX, int fromY, int toX, int toY,
        bool continuous = false)
    {
        mListener->move(fromX, fromY, toX, toY, continuous);
        if (!continuous)
        {
            mRound++;
            recreateLog();
        }
    }
    virtual void promoteMove(int fromX, int fromY, int toX, int toY,
        unsigned int promoteTo)
    {
        mListener->move(fromX, fromY, toX, toY, true, 0);
        mListener->move(toX, toY, toX, toY, false, promoteTo);
        mRound++;
        recreateLog();
    }
    virtual void setControl(bool white, bool black) { }

    virtual int getBoardWidth() const { return mBoardWidth; }
    virtual int getBoardHeight() const { return mBoardHeight; }
    virtual bool isWhiteTurn() const { return mRound % 2 == 0; }

    virtual void convertPosition(const Vector3& position, int* x, int* y) const;
    virtual Vector3 convertPosition(double x, double y) const;
    void createGround(bool visible);
    virtual SceneNode* createPiece(char type, const std::string& modelName, const Vector3& location, SceneNode* parent = 0);
    std::string getMeshName(char symbol) const;
    CEGUI::Window* createGUIComponent(const std::string& text, double x, double y,
        double sizeX, double sizeY, const std::string& type = "Button", bool setText = true, bool visible = true);
    virtual void setPromotionMove(int fromX, int fromY, int toX, int toY);
    void setChooseButtonsVisibility(bool visible);
    virtual void windowClosed(RenderWindow* rw);
    void ensureLatestState();
    virtual void recreateDeadPieces();
    virtual ~View();

protected:
    Root* mRoot;
    OIS::Keyboard* mKeyboard;
    OIS::Mouse* mMouse;
    OIS::InputManager* mInputManager;
    CEGUI::OgreCEGUIRenderer* mRenderer;
    CEGUI::System* mSystem;
    ViewFrameListener* mListener;
    SceneManager* mSceneMgr;
    Camera* mCamera;
    RenderWindow* mWindow;
    std::size_t mBoardWidth;
    std::size_t mBoardHeight;
    unsigned int mRound;
    std::vector<int> promotionMove;
    bool mPast;

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
    virtual void createCamera();
    virtual void createViewports();
    virtual void createInitialExplosion();
    virtual Entity* loadEntity(const std::string& entityName, const std::string& modelName);
    virtual void recreateLog();

    void createScene();
    void createGUI();
    void createBoard(const Board* board);

    bool undo(const CEGUI::EventArgs& e);
    bool restart(const CEGUI::EventArgs& e);
    bool visitSelectedLog(const CEGUI::EventArgs& e);
    bool chooseQueen(const CEGUI::EventArgs& e);
    bool chooseRook(const CEGUI::EventArgs& e);
    bool chooseKnight(const CEGUI::EventArgs& e);
    bool chooseBishop(const CEGUI::EventArgs& e);
    void sendPromotionMove(unsigned int promoteTo);
};

#endif // _NALCG_VIEW_H_

#ifndef _NALCG_BUFFERED_INPUT_HANDLER_H_
#define _NALCG_BUFFERED_INPUT_HANDLER_H_

#include <Ogre.h>
#include <CEGUI/CEGUI.h>
#include <OIS/OIS.h>

using namespace Ogre;

class View;
class AnimationManager;
class BufferedInputHandler : public OIS::KeyListener, public OIS::MouseListener
{
public:
    BufferedInputHandler(RenderWindow* window, Camera* camera, SceneManager* sceneMgr,
        AnimationManager* animationManager, View* view)
        : mLMouseDown(false), mRMouseDown(false), mWindow(window), mCamera(camera),
        mSceneMgr(sceneMgr), mRaySceneQuery(sceneMgr->createRayQuery(Ray())),
        mSelectedObject(0), mDirection(Vector3::ZERO), mAnimationManager(animationManager),
        mView(view), mSafeMode(true), mQueueAnimations(false), mCanShowSelectablePieces(true)
    {
    }

    ~BufferedInputHandler()
    {
        mSceneMgr->destroyQuery(mRaySceneQuery);
    }

    // KeyListener
    virtual bool keyPressed(const OIS::KeyEvent& arg);
    virtual bool keyReleased(const OIS::KeyEvent& arg);
    virtual CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID) const;
    virtual bool mouseMoved(const OIS::MouseEvent& arg);
    virtual bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
    virtual void moveCamera(const Real& timeSinceLastFrame);
    virtual void move(int fromX, int fromY, int toX, int toY,
        bool continuous = false, unsigned int promoteTo = 0);
    virtual void animationFinished();
    virtual bool showSelectablePieces();
    virtual void flagInvalidSquare();
    virtual bool canShowSelectablePieces()
    {
        return mCanShowSelectablePieces;
    }
    void setCanShowSelectablePieces(bool canShow)
    {
        mCanShowSelectablePieces = canShow;
    }
    virtual void clearSelectedObject()
    {
        mSelectedObject = 0;
    }

protected:
    bool mLMouseDown, mRMouseDown;     // True if the mouse buttons are down
    RenderWindow* mWindow;
    Camera* mCamera;
    SceneManager* mSceneMgr;           // A pointer to the scene manager
    RaySceneQuery* mRaySceneQuery;     // The ray scene query pointer
    SceneNode* mSelectedObject;         // The selected object
    Vector3 mDirection;     // Value to move in the correct direction
    AnimationManager* mAnimationManager;
    View* mView;
    bool mSafeMode;
    static const int CAMERA_MOVEMENT_SPEED = 500;
    std::deque<std::vector<int> > mAnimationQueue;
    bool mQueueAnimations;
    bool mCanShowSelectablePieces;

    virtual void onLeftPressed(const OIS::MouseEvent& arg);
    virtual SceneNode* findPieceAbove(Node* squareNode) const;
    virtual void onLeftReleased(const OIS::MouseEvent& arg);
    virtual void onRightPressed(const OIS::MouseEvent& arg);
    virtual void onRightReleased(const OIS::MouseEvent& arg);
    virtual bool toggleMovementPossibilities();
    virtual bool resetSquareIndicators(bool onlyInvalidsAndTargets = false);
    virtual void setMouseRay();
};

#endif // _NALCG_BUFFERED_INPUT_HANDLER_H_

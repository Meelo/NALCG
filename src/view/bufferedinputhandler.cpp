#include <OIS/OIS.h>

#include "bufferedinputhandler.h"
#include "animationmanager.h"
#include "animationfactory.h"
#include "movementanimation.h"
#include "../middleman.h"
#include "view.h"
#include "viewconstants.h"

bool BufferedInputHandler::keyPressed(const OIS::KeyEvent& arg)
{
    CEGUI::System* sys = CEGUI::System::getSingletonPtr();
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
    default:
        ; // No handling
    }
    return true;
}

bool BufferedInputHandler::keyReleased(const OIS::KeyEvent& arg)
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
    default:
        ; // No handling
    }
    return true;
}

// MouseListener

CEGUI::MouseButton BufferedInputHandler::convertButton(OIS::MouseButtonID buttonID) const
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
bool BufferedInputHandler::mouseMoved(const OIS::MouseEvent& arg)
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
bool BufferedInputHandler::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
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
bool BufferedInputHandler::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
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

void BufferedInputHandler::moveCamera(const Real& timeSinceLastFrame)
{
    mCamera->setPosition(mCamera->getPosition() + 
        mCamera->getOrientation() * mDirection * timeSinceLastFrame);
}

void BufferedInputHandler::onLeftPressed(const OIS::MouseEvent& arg)
{
    CEGUI::Point mousePos = CEGUI::MouseCursor::getSingleton().getPosition();

    Ray mouseRay = mCamera->getCameraToViewportRay(mousePos.d_x/arg.state.width, mousePos.d_y/arg.state.height);

    mRaySceneQuery->setRay(mouseRay);
    mRaySceneQuery->setSortByDistance(true);
    mRaySceneQuery->setQueryMask(1 << 0);

    RaySceneQueryResult& result = mRaySceneQuery->execute();
    RaySceneQueryResult::iterator itr;

    for (itr = result.begin(); itr != result.end(); itr++)
    {
        if (itr->movable)
        {
            if (mSelectedObject)
            {
                SceneNode* targetNode = itr->movable->getParentSceneNode();
                SceneNode* pieceNode = findPieceAbove(mSelectedObject);
                if (mSelectedObject != targetNode)
                {
                    std::cout << mSelectedObject->getName() << " -> " << targetNode->getName() << std::endl;

                    SceneNode* targetPiece = findPieceAbove(targetNode);

                    mAnimationManager->addAnimation(
                        AnimationFactory::createMovementAnimation(
                        *pieceNode->getName().begin(), targetNode->getPosition(),
                        pieceNode, targetPiece, mSceneMgr, mAnimationManager));
                }
                mSelectedObject->showBoundingBox(false);
                pieceNode->showBoundingBox(false); // FIXME: moving a dead unit causes the game to crash.
                Entity* ent = mSceneMgr->getEntity(mSelectedObject->getName() + "s");
                ent->setMaterialName("board/square/green");
                ent->setVisible(false);
                toggleMovementPossibilities();
                mSelectedObject = 0;
            }
            else
            {
                SceneNode* squareNode = itr->movable->getParentSceneNode();
                SceneNode* pieceNode = findPieceAbove(squareNode);
                if (pieceNode)
                {
                    mSelectedObject = squareNode;
                    mSelectedObject->showBoundingBox(true);
                    Entity* ent = mSceneMgr->getEntity(mSelectedObject->getName() + "s");
                    ent->setVisible(true);
                    ent->setMaterialName("board/square/cyan");
                    pieceNode->showBoundingBox(true);
                    toggleMovementPossibilities();
                }
            }
            break;
        }
    }
}

bool BufferedInputHandler::toggleMovementPossibilities()
{
    Middleman* middleman = mView->getMiddleman();
    if (!middleman)
    {
        return false;
    }

    int x = -1;
    int y = -1;
    convertPosition(mSelectedObject->getPosition(), &x, &y);

    std::vector<std::size_t> validMoves = middleman->getValidMovesAt(x, y);
    for (std::size_t i = 0; i < validMoves.size(); i++)
    {
        std::size_t column = 0;
        std::size_t row = 0;
        Board::getCoordinates(validMoves.at(i), column, row,
            mView->getBoardWidth(), mView->getBoardHeight());

        std::ostringstream name;
        name << "Board" << column << "," << row << "s";
        Entity* ent = mSceneMgr->getEntity(name.str());
        ent->setVisible(!ent->isVisible());
    }
    return true;
}

void BufferedInputHandler::convertPosition(const Vector3& position, int* x, int* y)
{
    int sideLength = ViewConstants::SQUARE_SIDE_LENGTH;
    int offsetX = (mView->getBoardWidth() - 1) * sideLength / 2;
    int offsetY = (mView->getBoardHeight() - 1) * sideLength / 2;
    // +0.5 for rounding.
    *x = (position.x + offsetX + 0.5) / sideLength;
    *y = (position.z + offsetY + 0.5) / sideLength;
}


SceneNode* BufferedInputHandler::findPieceAbove(SceneNode* squareNode) const
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

void BufferedInputHandler::onLeftReleased(const OIS::MouseEvent& arg)
{
}

void BufferedInputHandler::onRightPressed(const OIS::MouseEvent& arg)
{
}

void BufferedInputHandler::onRightReleased(const OIS::MouseEvent& arg)
{
}


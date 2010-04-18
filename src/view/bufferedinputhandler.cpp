#include "bufferedinputhandler.h"

#include "animationmanager.h"
#include "animationfactory.h"
#include "movementanimation.h"
#include "promotionanimation.h"
#include "../middleman.h"
#include "view.h"
#include "viewconstants.h"
#include "../logic/chessboard.h"
#include "checkanimation.h"

bool BufferedInputHandler::keyPressed(const OIS::KeyEvent& arg)
{
    CEGUI::System* sys = CEGUI::System::getSingletonPtr();
    sys->injectKeyDown(arg.key);
    sys->injectChar(arg.text);

    switch (arg.key)
    {
    case OIS::KC_1:
        mCamera->setPosition(ViewConstants::WHITE_CAMERA_POSITION);
        mCamera->lookAt(Vector3::ZERO);
        break;
    case OIS::KC_2:
        mCamera->setPosition(ViewConstants::BLACK_CAMERA_POSITION);
        mCamera->lookAt(Vector3::ZERO);
        break;
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
    bool processed = CEGUI::System::getSingleton().injectMouseButtonDown(convertButton(id));
    if (processed)
    {
        return true;
    }

    if (CEGUI::WindowManager::getSingleton().getWindow("View/Choose queenButton")->isVisible())
    {
        mView->setChooseButtonsVisibility(false);
    }

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
    bool processed = CEGUI::System::getSingleton().injectMouseButtonUp(convertButton(id));
    if (processed)
    {
        return true;
    }

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

    if (mCamera->getPosition().y < 10)
    {
        Vector3 newPosition = mCamera->getPosition();
        newPosition.y = 10;
        mCamera->setPosition(newPosition);
    }
}

void BufferedInputHandler::setMouseRay()
{
    CEGUI::Point mousePos = CEGUI::MouseCursor::getSingleton().getPosition();

    const OIS::MouseState& ms = mMouse->getMouseState();

    Ray mouseRay = mCamera->getCameraToViewportRay(
        mousePos.d_x/ms.width, mousePos.d_y/ms.height);

    mRaySceneQuery->setRay(mouseRay);
    mRaySceneQuery->setSortByDistance(true);
    mRaySceneQuery->setQueryMask(1 << 0);
}

void BufferedInputHandler::highlightHoveredSquare()
{
    resetSquareIndicators(true);

    setMouseRay();
    RaySceneQueryResult& result = mRaySceneQuery->execute();
    RaySceneQueryResult::iterator itr;

    for (itr = result.begin(); itr != result.end(); itr++)
    {
        if (itr->movable)
        {
            if (mSelectedObject)
            {
                SceneNode* targetNode = itr->movable->getParentSceneNode();
                Entity* ent = mSceneMgr->getEntity(targetNode->getName() + " s");
                const std::string& materialName = ent->getSubEntity(0)->getMaterialName();
                if (!ent->isVisible() && mMoveAssistanceLevel > 2)
                {
                    ent->setMaterialName("board/square/invalid");
                    ent->setVisible(true);
                }
                else if (materialName == "board/square/move")
                {
                    ent->setMaterialName("board/square/target/move");
                }
                else if (materialName == "board/square/attack")
                {
                    ent->setMaterialName("board/square/target/attack");
                }
            }
            break;
        }
    }
}

void BufferedInputHandler::onLeftPressed(const OIS::MouseEvent& arg)
{
    mView->ensureLatestState();

    setMouseRay();
    RaySceneQueryResult& result = mRaySceneQuery->execute();
    RaySceneQueryResult::iterator itr;

    for (itr = result.begin(); itr != result.end(); itr++)
    {
        if (itr->movable)
        {
            if (mSafeMode)
            {
                mAnimationManager->finishAnimations();
            }

            if (mSelectedObject)
            {
                SceneNode* targetNode = itr->movable->getParentSceneNode();
                if (mSelectedObject != targetNode)
                {
                    std::stringstream name;
                    name << mSelectedObject->getName() << " " << targetNode->getName();
                    std::size_t fromX, fromY, toX, toY;
                    name >> fromX;
                    name >> fromY;
                    name >> toX;
                    name >> toY;

                    Middleman* middleman = mView->getMiddleman();
                    if (middleman)
                    {
                        unsigned int returnValue = middleman->move(
                            fromX, fromY, toX, toY);

                        if (returnValue & Board::INVALID_MOVE
                            || returnValue & Board::INVALID_TURN)
                        {
                            SceneNode* node = mSceneMgr->getSceneNode("InvalidMove");
                            node->setVisible(true);
                            node->setPosition(targetNode->getPosition());
                            node->translate(0, 1, 0);
                            node->setScale(0, 1, 0);
                        }
                        if (returnValue & Board::PROMOTION_REQUEST)
                        {
                            mView->setPromotionMove(fromX, fromY, toX, toY);
                            mView->setChooseButtonsVisibility(true);
                        }
                    }
                    else
                    {
                        move(fromX, fromY, toX, toY);
                    }
                }
                Entity* ent = mSceneMgr->getEntity(mSelectedObject->getName() + " s");
                ent->setMaterialName("board/square/move");
                ent->setVisible(false);
                mSceneMgr->getSceneNode("Selection")->setVisible(false);
                mSelectedObject = 0;
                resetSquareIndicators();
                mCanShowSelectablePieces = true;
            }
            else
            {
                SceneNode* squareNode = itr->movable->getParentSceneNode();
                SceneNode* pieceNode = findPieceAbove(squareNode);
                if (pieceNode)
                {
                    resetSquareIndicators();
                    mSelectedObject = squareNode;
                    showMovementPossibilities();

                    SceneNode* selection = mSceneMgr->getSceneNode("Selection");
                    selection->setPosition(mSelectedObject->getPosition());
                    selection->translate(0, 1, 0);
                    selection->setVisible(true);
                    dynamic_cast<ParticleSystem*>(selection->getAttachedObject(0))->fastForward(5);

                    mCanShowSelectablePieces = false;
                }
            }
            break;
        }
    }
}

void BufferedInputHandler::move(int fromX, int fromY, int toX, int toY,
                                bool continuous, unsigned int promoteTo)
{
    if (mQueueAnimations && mAnimationManager->animationsRunning())
    {
        std::vector<int> moveOrder;
        moveOrder.push_back(fromX);
        moveOrder.push_back(fromY);
        moveOrder.push_back(toX);
        moveOrder.push_back(toY);
        moveOrder.push_back(continuous);
        moveOrder.push_back(promoteTo);

        mAnimationQueue.push_back(moveOrder);
    }
    else
    {
        std::ostringstream sourceName;
        sourceName << fromX << " " << fromY;
        SceneNode* pieceNode = findPieceAbove(mSceneMgr->getSceneNode(sourceName.str()));

        std::ostringstream targetName;
        targetName << toX << " " << toY;
        SceneNode* targetNode = mSceneMgr->getSceneNode(targetName.str());

        SceneNode* targetPiece = findPieceAbove(targetNode);

        GenericAnimation* animation;
        if (promoteTo)
        {
            animation = AnimationFactory::createPromotionAnimation(
                promoteTo, pieceNode, mSceneMgr, mView);
        }
        else
        {
            animation = AnimationFactory::createMovementAnimation(
                *pieceNode->getName().begin(), targetNode->getPosition(),
                pieceNode, targetPiece, mSceneMgr, mAnimationManager);
        }

        mAnimationManager->addAnimation(animation);

        animation->enableCallback(this);
        if (continuous)
        {
            mQueueAnimations = true;
        }
        else
        {
            mQueueAnimations = false;
        }
    }
}

void BufferedInputHandler::animationFinished()
{
    if (!mAnimationQueue.empty())
    {
        std::vector<int> moveOrder = mAnimationQueue.back();
        mAnimationQueue.pop_back();

        // fromX, fromY, toX, toY, continuous, promote
        // TODO: a struct or class for this so it won't seem so magical
        move(moveOrder.at(0), moveOrder.at(1),
            moveOrder.at(2), moveOrder.at(3),
            moveOrder.at(4) != 0, moveOrder.at(5));
    }
}

bool BufferedInputHandler::resetSquareIndicators(bool onlyInvalidsAndTargets)
{
    Middleman* middleman = mView->getMiddleman();
    if (!middleman)
    {
        return false;
    }
    for (int i = 0; i < mView->getBoardWidth(); i++)
    {
        for (int j = 0; j < mView->getBoardHeight(); j++)
        {
            std::ostringstream name;
            name << i << " " << j << " s";
            Entity* ent = mSceneMgr->getEntity(name.str());

            const std::string materialName = ent->getSubEntity(0)->getMaterialName();

            if (!onlyInvalidsAndTargets)
            {
                ent->setVisible(false);
                ent->setMaterialName("board/square/move");
            }
            else if (materialName == "board/square/invalid")
            {
                ent->setVisible(false);
            }
            else if (materialName == "board/square/target/move")
            {
                ent->setMaterialName("board/square/move");
            }
            else if (materialName == "board/square/target/attack")
            {
                ent->setMaterialName("board/square/attack");
            }
        }
    }
    return true;
}

bool BufferedInputHandler::showSelectablePieces()
{
    Middleman* middleman = mView->getMiddleman();
    if (!middleman)
    {
        return false;
    }
    mCanShowSelectablePieces = false;

    for (int i = 0; i < mView->getBoardWidth(); i++)
    {
        for (int j = 0; j < mView->getBoardHeight(); j++)
        {
            std::ostringstream name;
            name << i << " " << j << " s";
            Entity* ent = mSceneMgr->getEntity(name.str());

            if (ent->getParentNode())
            {
                SceneNode* pieceNode = findPieceAbove(ent->getParentNode()->getParent());
                if (pieceNode)
                {
                    bool whitePiece = pieceNode->getName().find("white") != std::string::npos;
                    bool whiteTurn = mView->isWhiteTurn();
                    if (((whitePiece && whiteTurn) || (!whitePiece && !whiteTurn))
                        && middleman->getValidMovesAt(i, j).size() > 0)
                    {
                        ent->setVisible(true);
                        ent->setMaterialName("board/square/selected");
                    }
                }
            }
        }
    }
    return true;
}

bool BufferedInputHandler::showMovementPossibilities()
{
    Entity* ent = mSceneMgr->getEntity(mSelectedObject->getName() + " s");
    ent->setVisible(true);
    ent->setMaterialName("board/square/selected");

    Middleman* middleman = mView->getMiddleman();
    if (!middleman)
    {
        return false;
    }

    int x = -1;
    int y = -1;
    mView->convertPosition(mSelectedObject->getPosition(), &x, &y);

    std::vector<std::size_t> validMoves = middleman->getValidMovesAt(x, y);
    for (std::size_t i = 0; i < validMoves.size(); i++)
    {
        std::size_t column = 0;
        std::size_t row = 0;
        Board::getCoordinates(validMoves.at(i), column, row,
            mView->getBoardWidth(), mView->getBoardHeight());

        std::ostringstream name;
        name << column << " " << row << " s";
        Entity* ent = mSceneMgr->getEntity(name.str());
        ent->setVisible(true);
        if (ent->getParentNode() && findPieceAbove(ent->getParentNode()->getParent()))
        {
            ent->setMaterialName("board/square/attack");
        }

    }
    return true;
}


SceneNode* BufferedInputHandler::findPieceAbove(Node* squareNode) const
{
    return findPieceAbove(squareNode, mSceneMgr);
}

SceneNode* BufferedInputHandler::findPieceAbove(Node* squareNode, SceneManager* sceneMgr)
{
    // TODO: When everything is done, we should do a null check here
    // for the squareNode in case query mask for some env is not correctly set

    const Vector3& squarePosition = squareNode->getPosition();
    Node::ChildNodeIterator it = sceneMgr->getRootSceneNode()->getChildIterator();
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

bool BufferedInputHandler::handleMoveAssistanceChanged(const CEGUI::EventArgs& e)
{
    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window* window = wmgr.getWindow("View/Move assistanceSpinner");
    CEGUI::Spinner* spinner = static_cast<CEGUI::Spinner*>(window);

    mMoveAssistanceLevel = spinner->getCurrentValue() + 0.5;

    resetSquareIndicators();
    if (mSelectedObject)
    {
        showMovementPossibilities();
    }
    else
    {
        mCanShowSelectablePieces = true;
    }
    return true;
}

bool BufferedInputHandler::handleSafeModeChanged(const CEGUI::EventArgs& e)
{
    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window* window = wmgr.getWindow("View/Unsafe modeCheckbox");
    CEGUI::Checkbox* checkbox = static_cast<CEGUI::Checkbox*>(window);

    mSafeMode = !checkbox->isSelected();
    return true;
}

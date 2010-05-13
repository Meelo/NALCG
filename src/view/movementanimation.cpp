#include "movementanimation.h"

#include "viewconstants.h"
#include "view.h"
#include "../logic/chessboard.h"
#include "../middleman.h"
#include "animationfactory.h"
#include "checkanimation.h"

MovementAnimation::MovementAnimation(
    const Vector3& destination,
    SceneNode *movingNode,
    SceneNode *targetPiece,
    SceneManager *sceneMgr,
    AnimationManager *animationManager)
    : GenericAnimation(movingNode, sceneMgr)
    , mDestination(destination)
    , mTargetPiece(targetPiece)
    , mTargetPieceName(targetPiece ? targetPiece->getName() : "NO_TARGET")
    , mAnimationManager(animationManager)
    , mIsCheck(false)
    , mIsCheckmate(false)
    , mIsWhiteTurn(mAnimationManager->getView()->isWhiteTurn())
{

    View* view = animationManager->getView();
    Middleman* middleman = view->getMiddleman();
    if (middleman)
    {
        mIsCheck = (middleman->getGameConditionMask() & ChessBoard::CHECK) != 0;
        mIsCheckmate = (middleman->getGameConditionMask() & ChessBoard::CHECKMATE) != 0;
    }
}

void MovementAnimation::dimLights()
{
    mSceneMgr->setAmbientLight(ColourValue(0.05, 0.05, 0.05));
    mSceneMgr->getLight("Yellow")->setDiffuseColour(0.05, 0.05, 0);
    mSceneMgr->getLight("Blue")->setDiffuseColour(0.0, 0.0, 0.05);
}

void MovementAnimation::restoreLights()
{
    mSceneMgr->setAmbientLight(ViewConstants::AMBIENT_COLOUR);
    mSceneMgr->getLight("Yellow")->setDiffuseColour(ViewConstants::YELLOW_COLOUR);
    mSceneMgr->getLight("Blue")->setDiffuseColour(ViewConstants::BLUE_COLOUR);
}

void MovementAnimation::playAnimation(const std::string& animationName, double time, bool stop, SceneNode* animatedNode, bool loop)
{
    if (!animatedNode)
    {
        animatedNode = mAnimatedNode;
    }
    SceneNode::ObjectIterator it = animatedNode->getAttachedObjectIterator();
    while (it.hasMoreElements())
    {
        MovableObject* obj = it.getNext();
        Entity* ent = dynamic_cast<Entity*>(obj);

        AnimationStateSet* animations = ent->getAllAnimationStates();
        if (animations && animations->hasAnimationState(animationName))
        {
            AnimationState* animationState = animations->getAnimationState(animationName);
            animationState->setEnabled(true);
            animationState->setLoop(loop);
            animationState->addTime(time);
            if (stop)
            {
                animationState->setTimePosition(0);
            }
        }
    }
}

MovementAnimation::~MovementAnimation()
{
    View* view = mAnimationManager->getView();

    // Make sure another shorter animation hasn't already destroyed the piece.
    // Using target piece name since target piece pointer might already be freed
    // and isn't safe to call.
    if (mSceneMgr->hasSceneNode(mTargetPieceName))
    {
        mAnimationManager->stopAllAnimationsBelongingTo(mTargetPiece);
        mSceneMgr->getRootSceneNode()->removeAndDestroyChild(mTargetPieceName);

        view->recreateDeadPieces();
    }

    Middleman* middleman = view->getMiddleman();

    if (middleman && mIsCheck)
    {
        const ChessBoard* board = dynamic_cast<const ChessBoard*>(
            middleman->getGameStateAt(middleman->getGameLog().size()));

        std::size_t location = board->findKing(mIsWhiteTurn ? WHITE : BLACK);

        std::size_t column;
        std::size_t row;
        ChessBoard::getCoordinates(location, column, row);

        std::ostringstream sourceName;
        sourceName << column << " " << row;
        SceneNode* pieceNode = BufferedInputHandler::findPieceAbove(
            mSceneMgr->getSceneNode(sourceName.str()), mSceneMgr);

        if (pieceNode && pieceNode != mAnimatedNode)
        {
            if (mIsCheckmate)
            {
                int x0 = -1;
                int y0 = -1;
                view->convertPosition(mAnimatedNode->getPosition(), &x0, &y0);
                int x1 = -1;
                int y1 = -1;
                view->convertPosition(pieceNode->getPosition(), &x1, &y1);
                view->move(x0, y0, x1, y1, true);
            }
            else
            {
                mAnimationManager->addAnimation(
                    AnimationFactory::createCheckAnimation(
                    pieceNode, mSceneMgr));
            }
        }
    }

}
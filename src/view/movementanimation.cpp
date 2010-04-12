#include "movementanimation.h"

#include "viewconstants.h"
#include "view.h"
#include "../logic/chessboard.h"
#include "../middleman.h"
#include "animationfactory.h"


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
    // Make sure another shorter animation hasn't already destroyed the piece.
    // Using target piece name since target piece pointer might already be freed
    // and isn't safe to call.
    if (mSceneMgr->hasSceneNode(mTargetPieceName))
    {
        mAnimationManager->stopAllAnimationsBelongingTo(mTargetPiece);
        mSceneMgr->getRootSceneNode()->removeAndDestroyChild(mTargetPieceName);

        View* view = mAnimationManager->getView();
        view->recreateDeadPieces();

        Middleman* middleman = view->getMiddleman();

        if (middleman->getGameConditionMask & ChessBoard::CHECK)
        {
            const ChessBoard* board = dynamic_cast<const ChessBoard*>(
                middleman->getGameStateAt(middleman->getGameLog().size()));

            std::size_t location = board->findKing(view->isWhiteTurn() ? WHITE : BLACK);

            std::size_t column;
            std::size_t row;
            ChessBoard::getCoordinates(location, column, row);

            std::ostringstream sourceName;
            sourceName << column << " " << row;
            SceneNode* pieceNode = BufferedInputHandler::findPieceAbove(
                mSceneMgr->getSceneNode(sourceName.str()), mSceneMgr);
            mAnimationManager->addAnimation(
                AnimationFactory::createCheckAnimation(
                pieceNode, mSceneMgr, 1.0));
        }
        //}
    }
}
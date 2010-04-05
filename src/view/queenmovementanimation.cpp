#include "queenmovementanimation.h"

#include "animationmanager.h"
#include "animationfactory.h"
#include "dyinganimation.h"
#include "bleedinganimation.h"

const int QueenMovementAnimation::MOVEMENT_SPEED = 600;
const int QueenMovementAnimation::ATTACK_ALTITUDE = 500;
const int QueenMovementAnimation::MOVE_ALTITUDE = 400;
const int QueenMovementAnimation::ATTACK_COUNT = 30;
const int QueenMovementAnimation::ATTACK_ANIMATION_LENGTH = 3;
const int QueenMovementAnimation::FLAT_ATTACKING_DISTANCE = 150;
const Real QueenMovementAnimation::ATTACK_COOLDOWN = 0.075;

bool QueenMovementAnimation::animate(const Real& timeSinceLastFrame)
{
    Real distanceMoved = MOVEMENT_SPEED * timeSinceLastFrame;
    Vector3 path = mDestination - mAnimatedNode->getPosition();
    if (path.length() > distanceMoved)
    {
        switch (mPhase)
        {
        case 1:
            // Fly higher in steep angle until flying altitude is reached.
            if (mAnimatedNode->getPosition().y > mFlyingAltitude)
            {
                mPhase = 2;
            }
            path += Vector3(0, mFlyingAltitude * 4, 0);
            break;
        case 2:
            // Continue flying on the same altitude until almost above target.
            if (path.length() < distanceMoved + mFlyingAltitude * 1.1)
            {
                mPhase = 3;
                if (mAttackCount > 0)
                {
                    dimLights();
                    mBloodCountdown = ATTACK_ANIMATION_LENGTH - 0.6;
                }
            }
            path += Vector3(0, mFlyingAltitude, 0);
            break;
        case 3:
        case 4:
            {
                if (mAttackCooldown <= 0 && mAttackCount > 0)
                {
                    SceneNode* animNode = mAnimatedNode->createChildSceneNode();

                    Animation* anim = mSceneMgr->createAnimation(nextName(), ATTACK_ANIMATION_LENGTH);
                    anim->setInterpolationMode(Animation::IM_SPLINE);
                    //mAnimations.push_back(anim);

                    NodeAnimationTrack* track = anim->createNodeTrack(1, animNode);
                    TransformKeyFrame* kf = track->createNodeKeyFrame(0);

                    //int waypoint = mAttackCount % 2 == 0 ? 100 : -100;
                    //int waypoint2 = mAttackCount % 4 <= 1 ? 100 : -100;
                    Real waypointX = 100 * sin(double(mAttackCount));
                    Real waypointY = 100 * cos(double(mAttackCount));
                    Real waypointX2 = waypointX * 4;
                    Real waypointY2 = waypointY * 4 + FLAT_ATTACKING_DISTANCE;
                    waypointY += FLAT_ATTACKING_DISTANCE;
                    kf = track->createNodeKeyFrame(0);
                    kf->setTranslate(Vector3(0, 150, 0));
                    //kf = track->createNodeKeyFrame(0.3);
                    //kf->setTranslate(Vector3(waypointX, waypointY, -100));
                    kf = track->createNodeKeyFrame(0.7);
                    kf->setTranslate(Vector3(waypointX, waypointY, -200));
                    kf = track->createNodeKeyFrame(1.8);
                    kf->setTranslate(Vector3(waypointX2, waypointY2, -400));
                    kf = track->createNodeKeyFrame(2.5);
                    kf->setTranslate(Vector3(0, FLAT_ATTACKING_DISTANCE, 400));
                    kf = track->createNodeKeyFrame(3);
                    kf->setTranslate(Vector3(0, FLAT_ATTACKING_DISTANCE, 600));

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
                    BillboardSet* bbs = mSceneMgr->createBillboardSet(nextName(), 1);
                    bbs->createBillboard(Vector3::ZERO, mTrail->getInitialColour(0));
                    bbs->setMaterialName("Examples/Flare");
                    bbs->setQueryFlags(0);
                    animNode->attachObject(bbs);

                    mAttackCooldown = ATTACK_COOLDOWN;
                    mAttackCount--;
                }

                // Advance the animations.
                std::vector<AnimationState*>::iterator animi;
                for (animi = mAnimStateList.begin(); animi != mAnimStateList.end(); ++animi)
                {
                    (*animi)->addTime(timeSinceLastFrame);
                }

                mBloodCountdown -= timeSinceLastFrame;
                if (mBloodCountdown < 0 && mSceneMgr->hasSceneNode(mTargetPieceName)
                    && mPhase == 3)
                {
                    mAnimationManager->addAnimation(
                        AnimationFactory::createBleedingAnimation(
                        mTargetPiece, mSceneMgr, 0, ATTACK_COOLDOWN * ATTACK_COUNT));

                    mAnimationManager->addAnimation(
                        AnimationFactory::createDyingAnimation(
                        mTargetPiece, mSceneMgr, ATTACK_COOLDOWN * ATTACK_COUNT * 3 / 4));

                    mBloodCountdown = std::numeric_limits<double>::infinity();
                }

                // Stay floating until attack animations have ended.
                if (mAttackCooldown > -ATTACK_ANIMATION_LENGTH)
                {
                    path += Vector3(0, mFlyingAltitude, 0);
                    if (path.length() - FLAT_ATTACKING_DISTANCE < distanceMoved)
                    {
                        distanceMoved = 0;
                        //mAnimatedNode->setPosition(mDestination + Vector3(0, mFlyingAltitude, 0));
                    }
                }
                else if (mPhase == 3)
                {
                    destroyAnimationsBillboardsAndTrails();
                    mPhase = 4;
                }
                mAttackCooldown -= timeSinceLastFrame;
            }
        }

        // Normalising the vector so the speed remains constant.


        //mAnimatedNode->setOrientation(mAnimatedNode->getInitialOrientation());
        if (distanceMoved > 0)
        {
            path.normalise();
            mAnimatedNode->translate(path * distanceMoved);

            mAnimatedNode->resetOrientation();
            Vector3 src = Vector3::UNIT_Z;
            Vector3 flatPath = path;
            flatPath.y = 0;
            mAnimatedNode->rotate(src.getRotationTo(flatPath));

            mAnimatedNode->pitch(Degree(mAnimatedNode->getPosition().y * 90.0 / mFlyingAltitude));
        }

        /*Vector3 src = mAnimatedNode->getOrientation() * Vector3::UNIT_Z;
        mAnimatedNode->rotate(src.getRotationTo(path));*/

        return true; // Animation still running.
    }

    mAnimatedNode->setPosition(mDestination);
    mAnimatedNode->setOrientation(mAnimatedNode->getInitialOrientation());
    return false; // Animation finished.
}

void QueenMovementAnimation::createBlasts()
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

void QueenMovementAnimation::destroyAnimationsBillboardsAndTrails()
{
    // FIXME: this destruction method is too excessive.
    // Causes the game to crash if this animation ends while others are playing.
    mSceneMgr->destroyAllAnimations();
    mSceneMgr->destroyAllAnimationStates();
    mSceneMgr->destroyAllBillboardChains();
    mSceneMgr->destroyAllBillboardSets();
    mSceneMgr->destroyAllRibbonTrails();
}




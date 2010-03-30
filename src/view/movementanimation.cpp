#include "movementanimation.h"
#include "viewconstants.h"

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
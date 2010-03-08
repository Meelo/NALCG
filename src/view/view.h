#include "ExampleApplication.h"

class TutorialApplication : public ExampleApplication
{
protected:
public:
    TutorialApplication()
    {
    }

    ~TutorialApplication()
    {
    }
protected:

    virtual void createCamera()
    {
        mCamera = mSceneMgr->createCamera("PlayerCam");
        // set its position, direction
        mCamera->setPosition(Vector3(0,10,500));
        mCamera->lookAt(Vector3(0,0,0));
        mCamera->setNearClipDistance(5);
    }

    virtual void createViewports()
    {
        // Create one viewport, entire window
        Viewport* vp = mWindow->addViewport(mCamera);
        vp->setBackgroundColour(ColourValue(0,0,0));
        // Alter the camera aspect ratio to match the viewport
        mCamera->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
    }

    void createScene()
    {
        Entity *ent;
        Light *light;

        mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));
        mSceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE);

        for (int i = 0; i < 8; i++) {
            std::stringstream name("BlackPawnEntity");
            name << i;
            ent = mSceneMgr->createEntity(name.str(), "black_pawn.mesh");
            ent->setCastShadows(true);
            mSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(-700 + i*200, 0, -500))->attachObject(ent);
        }

        ent = mSceneMgr->createEntity("BoardEntity", "board.mesh");
        mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ent);
        ent->setCastShadows(false);

        light = mSceneMgr->createLight("Light1");
        light->setType(Light::LT_POINT);
        light->setPosition(Vector3(150, 250, 150));
        light->setDiffuseColour(1.0, 1.0, 0.4);
        light->setSpecularColour(1.0, 1.0, 1.0);

        light = mSceneMgr->createLight("Light2");
        light->setType(Light::LT_POINT);
        light->setPosition(Vector3(-150, 250, -150));
        light->setDiffuseColour(0.4, 0.4, 1.0);
        light->setSpecularColour(1.0, 1.0, 1.0);
    }
};
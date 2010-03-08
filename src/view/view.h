#include "ExampleApplication.h"

class TutorialApplication : public ExampleApplication
{
protected:
public:
    TutorialApplication() : entityCount(0)
    {
    }

    ~TutorialApplication()
    {
    }
protected:
    int entityCount;

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

    virtual void createPiece(const std::string& modelName, const Vector3& location)
    {
        std::stringstream entityName(modelName);
        entityName << entityCount;
        entityCount++;

        Entity* ent = mSceneMgr->createEntity(entityName.str(), modelName);
        ent->setCastShadows(true);
        mSceneMgr->getRootSceneNode()->createChildSceneNode(location)->attachObject(ent);
    }

    void createScene()
    {
        Entity *ent;
        Light *light;

        mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));
        mSceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE);

        for (int i = 0; i < 8; i++)
        {
            createPiece("black_pawn.mesh", Vector3(-700 + i*200, 0, -500));
        }
        createPiece("black_rook.mesh", Vector3(-700, 0, -700));
        createPiece("black_knight.mesh", Vector3(-500, 0, -700));
        createPiece("black_bishop.mesh", Vector3(-300, 0, -700));
        createPiece("black_king.mesh", Vector3(-100, 0, -700));
        createPiece("black_queen.mesh", Vector3(100, 0, -700));
        createPiece("black_bishop.mesh", Vector3(300, 0, -700));
        createPiece("black_knight.mesh", Vector3(500, 0, -700));
        createPiece("black_rook.mesh", Vector3(700, 0, -700));

        for (int i = 0; i < 8; i++)
        {
            createPiece("white_pawn.mesh", Vector3(-700 + i*200, 0, 500));
        }
        createPiece("white_rook.mesh", Vector3(-700, 0, 700));
        createPiece("white_knight.mesh", Vector3(-500, 0, 700));
        createPiece("white_bishop.mesh", Vector3(-300, 0, 700));
        createPiece("white_king.mesh", Vector3(-100, 0, 700));
        createPiece("white_queen.mesh", Vector3(100, 0, 700));
        createPiece("white_bishop.mesh", Vector3(300, 0, 700));
        createPiece("white_knight.mesh", Vector3(500, 0, 700));
        createPiece("white_rook.mesh", Vector3(700, 0, 700));

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
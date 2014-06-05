#ifndef __World_h_
#define __World_h_
#include <string>


// Ogre header files
#include "Ogre.h"
#include "OgreMath.h"
#include "OgreSceneManager.h"
#include "OgreFontManager.h"
#include "OgreTextAreaOverlayElement.h"
#include "OgreSceneNode.h"
#include "OgreOverlayManager.h"
#include "OgreOverlay.h"

// IOS (Input system) header files
#include <ois/ois.h>

// Other input files for my project
#include "InputHandler.h"
#include "Geometry.h"



class World
{
public:
	
	static World *getSingleton();
	~World();
    void Think(float time);
	void DestroyInstance();
	void girlThrow();
	Ogre::SceneNode* getCarNode() {return car;}
	Ogre::SceneNode* getGirlNode() {return girl1;}
	Ogre::Real getCarSpeed() {return speed;}
	Ogre::Real getCarRadius() {return xSizeCar/2;}
	Ogre::Real getCarZRadius() {return zSizeCar/2;}
	void turnGirl(Ogre::Radian x);
	void gameOver();
protected:

	World();
	
	static World *_instance;

	Ogre::Real speed;

	Ogre::SceneNode *car;
	Ogre::SceneNode *road;
	Ogre::SceneNode *tire1;
	Ogre::SceneNode *tire2;
	Ogre::SceneNode *tire3;
	Ogre::SceneNode *tire4;

	Ogre::SceneNode *girl1;
	Ogre::SceneNode *girl2;
	Ogre::SceneNode *girl3;

	Ogre::SceneNode *tree1;
	Ogre::SceneNode *tree2;
	Ogre::SceneNode *tree3;



	Ogre::Entity *mCarEntity;
	Ogre::Entity *mTireEntity1;
	Ogre::Entity *mTireEntity2;
	Ogre::Entity *mTireEntity3;
	Ogre::Entity *mTireEntity4;

	Ogre::Entity *mGirlEntity1;
	Ogre::Entity *mGirlEntity2;
	Ogre::Entity *mGirlEntity3;
	Ogre::Entity *mRoadEntity;

	Ogre::Entity *mTreeEntity1;
	Ogre::Entity *mTreeEntity2;
	Ogre::Entity *mTreeEntity3;

	Ogre::AnimationState *mGirlAnimationState1;
	Ogre::AnimationState *mGirlAnimationState2;
	Ogre::AnimationState *mGirlAnimationState3;

	Ogre::Real xSizeCar;
	Ogre::Real ySizeCar;
	Ogre::Real zSizeCar;

};


#endif
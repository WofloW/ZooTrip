#ifndef __Food_h_
#define __Food_h_

#include "OgreSceneNode.h"
#include "OgreSceneManager.h"
#include "Ogre.h"
#include "Global.h"



class Food
{
public:
	Food(enum FoodName name, float speed);
	~Food(void);
	bool Think(float time);
private:
	enum FoodName mName;
	Ogre::Vector3 foodPosition;
	float lifeTime;
	float totalLifeTime;
	float mYSpeed;
	float mZSpeed;
	Ogre::SceneNode *foodNode;
	Ogre::Entity *foodEntity;
	Ogre::Entity *foodGroundEntity;
	bool onGround;
};

#endif
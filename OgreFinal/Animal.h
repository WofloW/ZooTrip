#ifndef Animal_h__
#define Animal_h__

#include "OgreSceneNode.h"
#include "OgreSceneManager.h"
#include "Ogre.h"
#include "Global.h"



class Animal
{
public:
	Animal(enum AnimalName name, Ogre::Vector3 pos);
	~Animal(void);
	Ogre::Vector3 getPosition() {return mPos;}
	Ogre::Real getRadius() {return radius;}
	bool Think(float time);
private:
	enum AnimalName mName;
	Ogre::SceneNode *aniNode1;

	Ogre::Entity *aniEntity1;

	Ogre::Vector3 mPos;
	Ogre::Vector3 mSpeed;
	enum FoodName likeFood;
	Ogre::AnimationState *mAnimalAnimationState1;

	Ogre::Real xSize;	//x,y,z when head points to the right
	Ogre::Real ySize;
	Ogre::Real zSize;
	Ogre::Real radius;
	bool leftPitch;
	Ogre::Real leftTimer;
	Ogre::Real leftCD;
};
#endif // Animal_h__


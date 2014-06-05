#ifndef Stone_h__
#define Stone_h__
#include "OgreSceneNode.h"
#include "OgreSceneManager.h"
#include "Ogre.h"


class Stone
{
public:
	Stone(Ogre::Vector3 pos);
	bool Think(float time);
	~Stone(void);
	Ogre::Vector3 getPosition() {return mPos;}
private:
	Ogre::SceneNode *stoneNode;
	Ogre::Entity *stoneEntity;
	Ogre::Vector3 mPos;
	Ogre::Real radius;

};

#endif // Stone_h__

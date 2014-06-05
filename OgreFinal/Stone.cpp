#include "Stone.h"
#include "OgreTrip.h"



Stone::Stone(Ogre::Vector3 pos)
{
	mPos = pos + Ogre::Vector3(0,-50, 0);
	stoneEntity = OgreTrip::getSceneMgr()->createEntity("rock.mesh");
	stoneNode = OgreTrip::getSceneMgr()->getRootSceneNode()->createChildSceneNode();
	stoneNode->attachObject(stoneEntity);
	stoneNode->setPosition(mPos);
	radius = 22;
}


Stone::~Stone(void)
{
	stoneNode->detachAllObjects();
	OgreTrip::getSceneMgr()->destroyEntity(stoneEntity);
	OgreTrip::getSceneMgr()->destroySceneNode(stoneNode);
}

bool Stone::Think(float time)
{
	if (AIManager::getSingleton()->calc_distance(mPos,  World::getSingleton()->getCarNode()->getPosition()) < pow(radius +  World::getSingleton()->getCarZRadius(),2) )
	{
		return true;
	}
	return false;
}

#include "Food.h"
#include "OgreTrip.h"

Food::Food(FoodName name, float speed)
{
	mName = name;
	mYSpeed = 20.0;
	totalLifeTime = 10;
	lifeTime = totalLifeTime;
	onGround = false;
	mZSpeed = speed;
	foodPosition = World::getSingleton()->getGirlNode()->_getDerivedPosition() + Ogre::Vector3(0, 5, 5);
	switch (name)
	{
	case GRASS:
		foodEntity = OgreTrip::getSceneMgr()->createEntity("grass.mesh");
		break;
	case FISH:
		foodEntity = OgreTrip::getSceneMgr()->createEntity("fish.mesh");
		break;
	case MEAT:
		foodEntity = OgreTrip::getSceneMgr()->createEntity("meat.mesh");
		break;
	default:
		foodEntity = OgreTrip::getSceneMgr()->createEntity("grass.mesh");
		break;
	}
	foodNode = OgreTrip::getSceneMgr()->getRootSceneNode()->createChildSceneNode();
	foodNode->attachObject(foodEntity);
	foodNode->setPosition(foodPosition);
	foodNode->setOrientation(World::getSingleton()->getGirlNode()->_getDerivedOrientation());
	foodNode->scale(3, 3, 3);
}


Food::~Food(void)
{
	foodNode->detachAllObjects();
	OgreTrip::getSceneMgr()->destroyEntity(foodEntity);
	if (foodGroundEntity)
	{
		OgreTrip::getSceneMgr()->destroyEntity(foodGroundEntity);
	}
	OgreTrip::getSceneMgr()->destroySceneNode(foodNode);
}

bool Food::Think(float time)
{
	float gravity = 100.0;

	if (onGround)
	{
		lifeTime -= time;
		if (lifeTime <= 0 || AIManager::getSingleton()->CheckEated(mName, foodPosition))
			return true;
	} 
	else
	{
		Ogre::Real ground_y = Geometry::getSingleton()->getGroundY(foodPosition.x, foodPosition.z);
		if (foodPosition.y >= ground_y + 8)
		{
			mYSpeed -= gravity * time;
			foodPosition += foodNode->getOrientation()*Ogre::Vector3(0,mYSpeed,mZSpeed)*time + Ogre::Vector3(0,0,World::getSingleton()->getCarSpeed()* time)  ;
			foodNode->setPosition(foodPosition);
		}
		else
		{
			mYSpeed = 0;
			mZSpeed = 0;
			switch (mName)
			{
			case FISH:
				foodGroundEntity = OgreTrip::getSceneMgr()->createEntity("ground_fish.mesh");
				break;
			case GRASS:
				foodGroundEntity = OgreTrip::getSceneMgr()->createEntity("ground_grass.mesh");
				break;
			case MEAT:
				foodGroundEntity = OgreTrip::getSceneMgr()->createEntity("ground_meat.mesh");
				break;
			default:
				foodGroundEntity = OgreTrip::getSceneMgr()->createEntity("ground_fish.mesh");
				break;
			}

			//foodGroundEntity->setCastShadows(false);
			foodNode->detachAllObjects();
			foodNode->attachObject(foodGroundEntity);
			onGround = true;
		}
	}
	return false;

}

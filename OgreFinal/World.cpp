#include "World.h"
#include "OgreTrip.h"





World::World()
{
	speed = 300;
	xSizeCar = 38.8;
	ySizeCar = 30.2;
	zSizeCar = 60.3;
	mGirlAnimationState1 = NULL;
	mCarEntity = OgreTrip::getSceneMgr()->createEntity("car.mesh");		
	mTireEntity1 = OgreTrip::getSceneMgr()->createEntity("tire.mesh");
	mTireEntity2 = OgreTrip::getSceneMgr()->createEntity("tire.mesh");
	mTireEntity3 = OgreTrip::getSceneMgr()->createEntity("tire.mesh");
	mTireEntity4 = OgreTrip::getSceneMgr()->createEntity("tire.mesh");
	mCarEntity->setCastShadows(true);
	mGirlEntity1 = OgreTrip::getSceneMgr()->createEntity("girl_throw1.mesh");	
	mGirlEntity2 = OgreTrip::getSceneMgr()->createEntity("girl_throw2.mesh");	
	mGirlEntity3 = OgreTrip::getSceneMgr()->createEntity("girl_throw3.mesh");	
	mGirlEntity1->setCastShadows(true);
	mGirlEntity2->setCastShadows(true);
	mGirlEntity3->setCastShadows(true);



	car = OgreTrip::getSceneMgr()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(30,270,0));
	car->attachObject(mCarEntity);
	tire1 = car->createChildSceneNode(Ogre::Vector3( 20, -15, -18 ));
	tire2 = car->createChildSceneNode(Ogre::Vector3( -21, -15, -18 ));
	tire3 = car->createChildSceneNode(Ogre::Vector3( 20, -15, 18 ));
	tire4 = car->createChildSceneNode(Ogre::Vector3( -21, -15, 18));
	tire1->attachObject(mTireEntity1);
	tire2->attachObject(mTireEntity2);
	tire3->attachObject(mTireEntity3);
	tire4->attachObject(mTireEntity4);

	Ogre::Vector3 mGirlPosition = Ogre::Vector3( 0, -5, 0 );
	girl1 = car->createChildSceneNode(mGirlPosition);
	girl2 = car->createChildSceneNode(mGirlPosition);
	girl3 = car->createChildSceneNode(mGirlPosition);
	girl1->attachObject(mGirlEntity1);
	girl2->attachObject(mGirlEntity2);
	girl3->attachObject(mGirlEntity3);
	girl1->scale(0.2, 0.2, 0.2);
	girl2->scale(0.2, 0.2, 0.2);
	girl3->scale(0.2, 0.2, 0.2);

	Ogre::SceneNode *mcameraNode = car->createChildSceneNode(Ogre::Vector3( 0, 60, -280 ));
	mcameraNode->attachObject(OgreTrip::getCamera());
	OgreTrip::getCamera()->lookAt(car->getPosition() + Ogre::Vector3(0, 60, 0));

	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);

	Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane, 160, 100000, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);

	mRoadEntity = OgreTrip::getSceneMgr()->createEntity("GroundEntity", "ground");
	road = OgreTrip::getSceneMgr()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0,270,0));
	road->attachObject(mRoadEntity);

	mRoadEntity->setMaterialName("Examples/Road");
	mRoadEntity->setCastShadows(false);

}


void
	World::girlThrow()
{
	mGirlAnimationState1 = mGirlEntity1->getAnimationState("girl_throw1");
	mGirlAnimationState1->setTimePosition(0);
	mGirlAnimationState1->setEnabled(true);
	mGirlAnimationState1->setLoop(false);
	mGirlAnimationState1->setWeight(1.0);

	mGirlAnimationState2 = mGirlEntity2->getAnimationState("girl_throw2");
	mGirlAnimationState2->setTimePosition(0);
	mGirlAnimationState2->setEnabled(true);
	mGirlAnimationState2->setLoop(false);
	mGirlAnimationState2->setWeight(1.0);

	mGirlAnimationState3 = mGirlEntity3->getAnimationState("girl_throw3");
	mGirlAnimationState3->setTimePosition(0);
	mGirlAnimationState3->setEnabled(true);
	mGirlAnimationState3->setLoop(false);
	mGirlAnimationState3->setWeight(1.0);
}

void
	World::turnGirl(Ogre::Radian x)
{
	girl1->setOrientation(Ogre::Quaternion(x, Ogre::Vector3::UNIT_Y));
	girl2->setOrientation(Ogre::Quaternion(x, Ogre::Vector3::UNIT_Y));
	girl3->setOrientation(Ogre::Quaternion(x, Ogre::Vector3::UNIT_Y));
}



void 
	World::Think(float time)
{

	if (mGirlAnimationState1 != NULL)
	{
		if(mGirlAnimationState1->getEnabled())
		{
			mGirlAnimationState1->addTime(time);
			mGirlAnimationState2->addTime(time);
			mGirlAnimationState3->addTime(time);
		}
	}

	Vector3 carPos = car->getPosition();
	Vector3 roadPos = road->getPosition();
	const Real distanceAboveTerrain = 25;
	const Real roadDistanceAboveTerrain = 3;
	car->setPosition( carPos.x,  Geometry::getSingleton()->getGroundY(carPos.x,carPos.z) + distanceAboveTerrain, carPos.z);
	road->setPosition(roadPos.x,  Geometry::getSingleton()->getGroundY(roadPos.x,roadPos.z) + roadDistanceAboveTerrain, roadPos.z);
	if (car->getPosition().z > 10e6)
	{
		car->setPosition(car->getPosition() + Ogre::Vector3(0,0,-10e6));
		road->setPosition(road->getPosition() + Ogre::Vector3(0,0,-10e6));
	}
	car->setPosition(car->getOrientation()*Ogre::Vector3( 0, 0, speed)* time + car->getPosition());
	road->setPosition(road->getOrientation()*Ogre::Vector3( 0, 0, speed)* time + road->getPosition());
	tire1->setOrientation(tire1->getOrientation()*Ogre::Quaternion(Ogre::Degree(speed*1.1)*time ,Ogre::Vector3::UNIT_X));
	tire2->setOrientation(tire2->getOrientation()*Ogre::Quaternion(Ogre::Degree(speed*1.1)*time ,Ogre::Vector3::UNIT_X));
	tire3->setOrientation(tire3->getOrientation()*Ogre::Quaternion(Ogre::Degree(speed*1.1)*time ,Ogre::Vector3::UNIT_X));
	tire4->setOrientation(tire4->getOrientation()*Ogre::Quaternion(Ogre::Degree(speed*1.1)*time ,Ogre::Vector3::UNIT_X));

}

World *World::_instance = 0;

World*
	World::getSingleton()
{
	if (!_instance)
	{
		_instance = new World();
	}
	return _instance;
}

World::~World() {
	DestroyInstance();
}


void
	World::DestroyInstance() {
		delete _instance;
		_instance = 0;
}

void World::gameOver()
{
	const Real distanceAboveTerrain = 25;
	const Real roadDistanceAboveTerrain = 3;
	car->setPosition( 30,  270 + distanceAboveTerrain, 0);
	road->setPosition(0,  270 + roadDistanceAboveTerrain, 0);
}
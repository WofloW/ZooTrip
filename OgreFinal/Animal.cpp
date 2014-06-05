#include "Animal.h"
#include "OgreTrip.h"



Animal::Animal(AnimalName name, Ogre::Vector3 pos)
{
	mName = name;
	mPos = pos;
	leftPitch = true;
	leftTimer = 0.25;
	leftCD = 0.5;
	
	
	switch (mName)
	{
	case GAZELLE:
		aniEntity1 = OgreTrip::getSceneMgr()->createEntity("gazelle.mesh");
		likeFood = GRASS;
		mSpeed = 100;
		xSize = 55.4;
		ySize = 64.4;
		zSize = 13.8;
		break;
	case OSTRICH:
		aniEntity1 = OgreTrip::getSceneMgr()->createEntity("ostrich.mesh");
		likeFood = GRASS;
		xSize = 34.3;
		ySize = 48.5;
		zSize = 25.1;
		break;
	case GOOSE:
		aniEntity1 = OgreTrip::getSceneMgr()->createEntity("goose.mesh");
		likeFood = FISH;
		xSize = 18;
		ySize = 20.8;
		zSize = 17.6;
		break;
	case DUCK:
		aniEntity1 = OgreTrip::getSceneMgr()->createEntity("duck.mesh");
		likeFood = FISH;
		xSize = 13.3;
		ySize = 18;
		zSize = 12.1;
		break;
	case BIRD:
		aniEntity1 = OgreTrip::getSceneMgr()->createEntity("bird.mesh");
		likeFood = FISH;
		xSize = 21.3;
		ySize = 20;
		zSize = 10.9;
		break;
	case CROCODILE:
		aniEntity1 = OgreTrip::getSceneMgr()->createEntity("crocodile.mesh");
		likeFood = MEAT;
		xSize = 103.5;
		ySize = 23;
		zSize = 70.7;
		break;
	default:
		aniEntity1 = OgreTrip::getSceneMgr()->createEntity("gazelle.mesh");
		likeFood = GRASS;
		xSize = 55.4;
		ySize = 74.4;
		zSize = 13.8;
		break;
	}
	radius = xSize/2;

	aniNode1 = OgreTrip::getSceneMgr()->getRootSceneNode()->createChildSceneNode();
	aniNode1->attachObject(aniEntity1);
	aniNode1->setPosition(mPos);
	
	if (pos.x > 0)
	{
		mSpeed = Ogre::Vector3(-40, 0, 0);
	} 
	else
	{
		mSpeed = Ogre::Vector3(40, 0, 0);
		aniNode1->yaw(Ogre::Degree(180));
	}

	Ogre::BillboardSet * m_pBillboardSet = OgreTrip::getSceneMgr()->createBillboardSet();
	m_pBillboardSet->setRenderingDistance(10000);
	switch (likeFood)
	{
	case GRASS:
		m_pBillboardSet->setMaterialName( "Examples/FoodGrass");
		break;
	case FISH:
		m_pBillboardSet->setMaterialName( "Examples/FoodFish");
		break;
	case MEAT:
		m_pBillboardSet->setMaterialName( "Examples/FoodMeat");
		break;
	default:
		break;
	}

	Ogre::Billboard *m_pBillboard_Food = m_pBillboardSet->createBillboard( Ogre::Vector3(0,ySize + 5,0));
	m_pBillboard_Food->setDimensions(30, 20);

	Ogre::SceneNode *billNode = aniNode1->createChildSceneNode();
	billNode->attachObject(m_pBillboardSet);
	
	/*
		mAnimalAnimationState1 = aniEntity1->getAnimationState("run");
		mAnimalAnimationState1->setTimePosition(0);
		mAnimalAnimationState1->setEnabled(true);
		mAnimalAnimationState1->setLoop(true);
		mAnimalAnimationState1->setWeight(1.0);
	*/
	
	
}


Animal::~Animal(void)
{

	aniNode1->detachAllObjects();
	OgreTrip::getSceneMgr()->destroyEntity(aniEntity1);
	OgreTrip::getSceneMgr()->destroySceneNode(aniNode1);
	
	
}

bool Animal::Think(float time)
{

	Real distanceAboveTerrain = 25;

	if (mName = CROCODILE)
	{
		distanceAboveTerrain = 5;
	}
	mPos = Ogre::Vector3(mPos.x,  Geometry::getSingleton()->getGroundY(mPos.x,mPos.z) + distanceAboveTerrain, mPos.z) + mSpeed * time;
	aniNode1->setPosition(mPos);
	if (leftPitch)
	{
		//aniNode1->roll(Ogre::Degree(time*3));
		aniNode1->pitch(Ogre::Degree(time*30));
		if (leftTimer <= 0)
		{
			leftPitch = false;
			leftTimer = leftCD;
		} 
		else
		{
			leftTimer -= time;
		}
		
	}
	else
	{
		//aniNode1->roll(Ogre::Degree(-time*3));
		aniNode1->pitch(Ogre::Degree(-time*30));
		if (leftTimer <= 0)
		{
			leftPitch = true;
			leftTimer = leftCD;
		} 
		else
		{
			leftTimer -= time;
		}
	}
	if (AIManager::getSingleton()->calc_distance(mPos,  World::getSingleton()->getCarNode()->getPosition()) < pow(radius +  World::getSingleton()->getCarRadius(),2) )
	{
		return true;
	}

	return false;
}


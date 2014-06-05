#include "AIManager.h"
#include "OgreTrip.h"

AIManager::AIManager(void)
{
	spawnAnimalCD = 3;
	spawnAnimalTimer = spawnAnimalCD;
	spawnStoneCD = 5;
	spawnStoneTimer = spawnStoneCD;

	FoodEatlist[GRASS].push_back(GAZELLE);
	FoodEatlist[GRASS].push_back(OSTRICH);
	FoodEatlist[FISH].push_back(GOOSE);
	FoodEatlist[FISH].push_back(DUCK);
	FoodEatlist[FISH].push_back(BIRD);
	FoodEatlist[MEAT].push_back(CROCODILE);
	//debugAnimal();
}


void 
	AIManager::AddFood(FoodName name, float speed)
{
	mFoodList.push_back(new Food( name , speed));
}

void 
	AIManager::AddAnimal(AnimalName name, Ogre::Vector3 pos)
{
	mAnimalList[name].push_back(new Animal(name, pos));
}

void 
	AIManager::AddStone(Ogre::Vector3 pos)
{
	mStoneList.push_back(new Stone(pos));
}


void
	AIManager::Think(float time)
{
	
	if (OgreTrip::level <= 5)
	{
		spawnAnimalCD = 3 - OgreTrip::level*0.2;
		spawnStoneCD = 5 - OgreTrip::level*0.4;
	}
	
	spawnAnimal(time);
	spawnStone(time);
	for (std::size_t i = 0; i < mFoodList.size(); i++ )
	{
		if (mFoodList[i]->Think(time))
		{
			delete mFoodList[i];
			mFoodList.erase(mFoodList.begin()+i);
		}
	}
	for (int m = 0; m < animalNum; m++)
	{
		for (std::size_t i = 0; i < mAnimalList[m].size(); i++ )
		{
			if (mAnimalList[m][i]->Think(time))
			{
				gameOver();
				break;
			}
			if (mAnimalList[m][i]->getPosition().z < World::getSingleton()->getCarNode()->getPosition().z -100)
			{
				delete mAnimalList[m][i];
				mAnimalList[m].erase(mAnimalList[m].begin()+i);
			}
		}
	}
	for (std::size_t i = 0; i < mStoneList.size(); i++ )
	{
		if (mStoneList[i]->Think(time))
		{
			gameOver();
			break;
		}
		if (mStoneList[i]->getPosition().z < World::getSingleton()->getCarNode()->getPosition().z -100)
		{
			delete mStoneList[i];
			mStoneList.erase(mStoneList.begin()+i);
		}
	}
}


void
	AIManager::spawnAnimal(float time2)
{
	srand (time(NULL));
	spawnAnimalTimer-= time2;
	if (spawnAnimalTimer <= 0)
	{
		spawnAnimalTimer = spawnAnimalCD;
		float x = ((rand()%2 * 2) - 1) * 400;		//10 seconds to reach the car
		float z = 10*World::getSingleton()->getCarSpeed() + World::getSingleton()->getCarNode()->getPosition().z;
		AddAnimal(AnimalName(rand()%animalNum), Ogre::Vector3(x, Geometry::getSingleton()->getGroundY(x,z), z));
	}
}
void
	AIManager::spawnStone(float time2)
{
	srand (time(NULL));
	spawnStoneTimer-= time2;
	if (spawnStoneTimer <= 0)
	{
		spawnStoneTimer = spawnStoneCD;
		float x = ((rand()%2 * 2) - 1) * 30;
		float z = 2000 + World::getSingleton()->getCarNode()->getPosition().z;
		AddStone(Ogre::Vector3(x, Geometry::getSingleton()->getGroundY(x,z)+80, z));
	}
}

void
	AIManager::debugAnimal()
{
	AddAnimal(GAZELLE, Ogre::Vector3(100, 270, 200));
	AddAnimal(OSTRICH, Ogre::Vector3(-100, 270, 200));
	AddAnimal(GOOSE, Ogre::Vector3(200, 270, 200));
	AddAnimal(DUCK, Ogre::Vector3(200, 270, 200));
	AddAnimal(BIRD, Ogre::Vector3(100, 270, 100));
	AddAnimal(CROCODILE, Ogre::Vector3(-100, 270, 100));
	AddStone(Ogre::Vector3(-100, 270, 200));
}

AIManager *AIManager::_instance = 0;

AIManager*
	AIManager::getSingleton()
{
	if (!_instance)
	{
		_instance = new AIManager();
	}
	return _instance;
}

AIManager::~AIManager() {
	DestroyInstance();
}


void
	AIManager::DestroyInstance() {
		delete _instance;
		_instance = 0;
}

bool 
	AIManager::CheckEated(FoodName name, Ogre::Vector3 pos)
{
	for (std::size_t m = 0; m < FoodEatlist[name].size(); m++ )
	{
		AnimalName aniname = FoodEatlist[name][m];
		for (std::size_t i = 0; i < mAnimalList[aniname].size(); i++ )
		{
			if (calc_distance(mAnimalList[aniname][i]->getPosition(), pos) < 20000) //(mAnimalList[aniname][i]->getRadius() + 25))
			{
				delete mAnimalList[aniname][i];
				mAnimalList[aniname].erase(mAnimalList[aniname].begin()+i);
				return true;
			}
		}
	}
	return false;
}

float
	AIManager::calc_distance(Ogre::Vector3 v1, Ogre::Vector3 v2)
{
	float x1 = v1.x;
	float z1 = v1.z;
	float x2 = v2.x;
	float z2 = v2.z;
	return  (pow((x1-x2),2) + pow(z1-z2,2));
}

void AIManager::gameOver()
{

	for (std::size_t m = 0; m < animalNum; m++ )
	{
		for (std::size_t i = 0; i < mAnimalList[m].size(); i++ )
		{

			delete mAnimalList[m][i];
		}
		mAnimalList[m].clear();
	}
	for (std::size_t i = 0; i<mFoodList.size(); i++)
	{
		delete mFoodList[i];
	}
	mFoodList.clear();
	for (std::size_t i = 0; i<mStoneList.size(); i++)
	{
		delete mStoneList[i];
	}
	mStoneList.clear();
	spawnAnimalTimer = spawnAnimalCD;
	spawnStoneTimer = spawnStoneCD;
	World::getSingleton()->gameOver();
	InputHandler::getSingleton()->gameOver();
	OgreTrip::pause = true;
	OgreTrip::gameStart = false;
}


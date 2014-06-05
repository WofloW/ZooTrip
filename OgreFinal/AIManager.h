#ifndef AIManager_h__
#define AIManager_h__



#include <vector>
#include "Food.h"
#include "Animal.h"
#include "Global.h"
#include "Stone.h"

#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

class AIManager
{
public:
	AIManager(void);
	~AIManager(void);
	void AddFood(FoodName name, float speed);
	void Think(float time);
	static AIManager *getSingleton();
	void DestroyInstance();
	bool CheckEated(FoodName name, Ogre::Vector3 pos);
	float calc_distance(Ogre::Vector3 v1, Ogre::Vector3 v2);
	void AddAnimal(AnimalName name, Ogre::Vector3 pos);
	void spawnAnimal(float time2);
	void debugAnimal();
	void spawnStone(float time2);
	void gameOver();
	void AddStone(Ogre::Vector3 pos);
private:
	static AIManager *_instance;
	std::vector<Food*> mFoodList;
	static const int animalNum = 6;
	static const int foodNum = 3;
	std::vector<Animal*> mAnimalList[animalNum];
	std::vector<AnimalName> FoodEatlist[foodNum];
	std::vector<Stone*> mStoneList;
	Ogre::Real spawnAnimalCD;
	Ogre::Real spawnAnimalTimer;
	Ogre::Real spawnStoneCD;
	Ogre::Real spawnStoneTimer;
};

#endif // AIManager_h__
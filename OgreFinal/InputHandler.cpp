#include "InputHandler.h"
#include "OgreTrip.h"
#include <fstream>

InputHandler::InputHandler()
{
	mGoingForward = false;
	mGoingBack = false;
	mGoingLeft = false;
	mGoingRight = false;
	addPower = false;
	helpCreated = false;
	throwSound = "throw.mp3";
	loseSound = "lose.mp3";
	mTopSpeed = 400;
	power = 40.0;
	unitPower = 300.0;
	selectFood = GRASS;
	meter = 0;

	mRenderWindow = OgreTrip::getWindow();
	mInfo["Help"] =
		"1. Left click - Throw. press longer the further\n"
		"2. Middle mouse wheel - Zoom in or out\n"
		"3. Right click - Switch the road\n"
		"4. Press 1, 2, 3 - Switch food\n";

	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mRenderWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
	pl.insert(std::make_pair("x11_keyboard_grab", "false"));
	pl.insert(std::make_pair("x11_mouse_grab", "false"));
	pl.insert(std::make_pair("w32_mouse", "DISCL_FOREGROUND"));
	pl.insert(std::make_pair("w32_mouse", "DISCL_EXCLUSIVE"));
	pl.insert(std::make_pair("w32_keyboard", "DISCL_FOREGROUND"));
	pl.insert(std::make_pair("w32_keyboard", "DISCL_NONEXCLUSIVE"));

	mInputManager = OIS::InputManager::createInputSystem( pl );

	//mCurrentKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true));
	//mCurrentMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));
	//mCurrentKeyboard->setEventCallback( this );
	//mCurrentMouse->setEventCallback( this );


	mInputContext.mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
	mInputContext.mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));

	OIS::MouseState &mouseState = const_cast<OIS::MouseState &>(mInputContext.mMouse->getMouseState());
	mouseState.width = mRenderWindow->getWidth();
	mouseState.height = mRenderWindow->getHeight();
	mouseState.X.abs = mRenderWindow->getWidth()/2;
	mouseState.Y.abs = mRenderWindow->getHeight()/2;

	mInputContext.mKeyboard->setEventCallback(this);
	mInputContext.mMouse->setEventCallback(this);

	Ogre::ResourceManager::ResourceMapIterator iter = Ogre::FontManager::getSingleton().getResourceIterator();
	while (iter.hasMoreElements()) 
	{ 
		iter.getNext()->load(); 
	}
	mTrayMgr = new OgreBites::SdkTrayManager("SampleControls", OgreTrip::getWindow(), mInputContext, this);  // create a tray interface
	setupUI();
	m_pBillboardSet = OgreTrip::getSceneMgr()->createBillboardSet();
	m_pBillboardSet->setRenderingDistance(500);
	m_pBillboardSet->setMaterialName( "Examples/StrengthBar");
	m_pBillboard_StrengthBar = m_pBillboardSet->createBillboard(Ogre::Vector3(15,40,0) );
	m_pBillboardSet->setBillboardOrigin(BBO_TOP_LEFT);
	billNode = World::getSingleton()->getCarNode()->createChildSceneNode();
	billNode->attachObject(m_pBillboardSet);
}



void 
	InputHandler::Think(float time)
{
	mInputContext.capture();
	//mCurrentKeyboard->capture();
	//mCurrentMouse->capture();
	if(!OgreTrip::gameStart && !helpCreated)
	{
		mTrayMgr->createDecorWidget(OgreBites::TL_CENTER, "StartHelp", "Examples/StartHelp");
		mTrayMgr->createButton(OgreBites::TL_CENTER, "StartButton", "start", 150);
		helpCreated = true;
	}
	if (!mTrayMgr->isDialogVisible())
	{
		if (mDetailsPanel->isVisible())   // if details panel is visible, then update its contents
		{
			/*
			mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(OgreTrip::getCamera()->getPosition().x));
			mDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(OgreTrip::getCamera()->getPosition().y));
			mDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(OgreTrip::getCamera()->getPosition().z));
			mDetailsPanel->setParamValue(3, Ogre::StringConverter::toString(OgreTrip::getCamera()->getOrientation().w));
			mDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(OgreTrip::getCamera()->getOrientation().x));
			mDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(OgreTrip::getCamera()->getOrientation().y));
			mDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(OgreTrip::getCamera()->getOrientation().z));
			mDetailsPanel->setParamValue(7, Ogre::StringConverter::toString(mousez));
			*/
			mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString((int)meter));
			mDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(OgreTrip::level));
		}
	}
	mVelocity = Ogre::Vector3::ZERO;
	if (mGoingForward) mVelocity += OgreTrip::getCamera()->getDirection();
	if (mGoingBack) mVelocity -= OgreTrip::getCamera()->getDirection();
	if (mGoingRight) mVelocity += OgreTrip::getCamera()->getRight();
	if (mGoingLeft) mVelocity -= OgreTrip::getCamera()->getRight();
	

	// debug move camera
	if (mVelocity != Ogre::Vector3::ZERO) OgreTrip::getCamera()->move(mVelocity * mTopSpeed* time);

	if (addPower)
	{
		power += time * unitPower;
	}
	
	m_pBillboard_StrengthBar->setDimensions((power-300)/50, 5);
	

	if (mTrayMgr->isDialogVisible())
		OgreTrip::pause = true;
	else if(OgreTrip::gameStart)
		OgreTrip::pause = false;
	if (!OgreTrip::pause)
	{
		meter += time* World::getSingleton()->getCarSpeed()/5;
		if (meter != 0)
			OgreTrip::level = (int)(meter / (World::getSingleton()->getCarSpeed() * 20)) + 1;
	}
	
}

bool
	InputHandler::IsKeyDown(OIS::KeyCode key)
{
	return mInputContext.mKeyboard->isKeyDown(key);
}



bool 
	InputHandler::mouseMoved(const OIS::MouseEvent &e)
{

	Ogre::Real offset_x = e.state.width/2 - e.state.X.abs;
	if (offset_x > 240) offset_x = 240;
	if (offset_x < -240) offset_x = -240;

	//OgreTrip::getCamera()->setOrientation(Ogre::Quaternion(Ogre::Radian(Ogre::Math::PI) + Ogre::Math::ATan((offset_x)/216),  Ogre::Vector3::UNIT_Y));
	World::getSingleton()->turnGirl(Ogre::Math::ATan((offset_x/2)/280));

	if (-e.state.Z.rel < 0 && OgreTrip::getCamera()->getPosition().z < 100)
	{
		OgreTrip::getCamera()->moveRelative(Ogre::Vector3(0,0, -e.state.Z.rel));
	} 
	else if (-e.state.Z.rel > 0 && OgreTrip::getCamera()->getPosition().z > -100)
	{
		OgreTrip::getCamera()->moveRelative(Ogre::Vector3(0,0, -e.state.Z.rel));
	}
	
	
	if (mTrayMgr->injectMouseMove(e)) return true;
	return false;
}

bool 
	InputHandler::mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID button)
{
	if (!OgreTrip::pause)
	{
		if (button == OIS::MB_Left)
		{
			addPower = true;
		}

		else if (button == OIS::MB_Right)
		{
			Ogre::SceneNode* car = World::getSingleton()->getCarNode();
			Ogre::Vector3 carPos = car->getPosition();
			car->setPosition(-carPos.x, carPos.y, carPos.z);
		}
	}
	
	if (mTrayMgr->injectMouseDown(e, button)) return true;
	return true;
}

bool 
	InputHandler::mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID button)
{
	if (!OgreTrip::pause)
	{
		if (button == OIS::MB_Left)
		{
			addPower = false;
			OgreTrip::getSoundEng()->play2D(throwSound.c_str());
			World::getSingleton()->girlThrow();
			AIManager::getSingleton()->AddFood(selectFood, power);
			power = 300.0;
		}
	}
	
	if (mTrayMgr->injectMouseUp(e, button)) return true;
	return false;
}

bool 
	InputHandler::keyPressed (const OIS::KeyEvent &e)

{
	switch (e.key)
	{
	case OIS::KC_UP:
		{
			mGoingForward = true;
		}
		break;
	case OIS::KC_DOWN:
		{
			mGoingBack = true;
		}
		break;

	case OIS::KC_LEFT:
		{
			mGoingLeft = true;
		}
		break;
	case OIS::KC_RIGHT:
		{
			mGoingRight = true;
		}
		break;
	case OIS::KC_H:
	case OIS::KC_F1:
		{
			
			if (!mTrayMgr->isDialogVisible() && mInfo["Help"] != "") mTrayMgr->showOkDialog("Help", mInfo["Help"]);
			else mTrayMgr->closeDialog();
		}
		break;
	case OIS::KC_1:
		{
			if (selectFood != GRASS)
			{
				if (selectFood == MEAT)
					mTrayMgr->destroyWidget(OgreBites::TL_BOTTOM, "meat");
				else if (selectFood == FISH)
					mTrayMgr->destroyWidget(OgreBites::TL_BOTTOM, "fish");
				mTrayMgr->createDecorWidget(OgreBites::TL_BOTTOM, "grass", "Examples/SelectGrass");
			}
			selectFood = GRASS;
		}
		break;
	case OIS::KC_2:
		{
			if (selectFood != FISH)
			{
				if (selectFood == GRASS)
					mTrayMgr->destroyWidget(OgreBites::TL_BOTTOM, "grass");
				else if (selectFood == MEAT)
					mTrayMgr->destroyWidget(OgreBites::TL_BOTTOM, "meat");
				mTrayMgr->createDecorWidget(OgreBites::TL_BOTTOM, "fish", "Examples/SelectFish");
			}
			selectFood = FISH;
		}
		break;
	case OIS::KC_3:
		{
			if (selectFood != MEAT)
			{
				if (selectFood == GRASS)
					mTrayMgr->destroyWidget(OgreBites::TL_BOTTOM, "grass");
				else if (selectFood == FISH)
					mTrayMgr->destroyWidget(OgreBites::TL_BOTTOM, "fish");
				mTrayMgr->createDecorWidget(OgreBites::TL_BOTTOM, "meat", "Examples/SelectMeat");
			}
			selectFood = MEAT;
		}
		break;
	default:
		break;
	}

	return true;
}

bool 
	InputHandler::keyReleased(const OIS::KeyEvent &e)
{
	switch (e.key)
	{
	case OIS::KC_UP:
		{
			mGoingForward = false;
		}
		break;
	case OIS::KC_DOWN:
		{
			mGoingBack = false;
		}
		break;

	case OIS::KC_LEFT:
		{
			mGoingLeft = false;
		}
		break;
	case OIS::KC_RIGHT:
		{
			mGoingRight = false;
		}
		break;
	default:
		break;
	}

	return true;
}

void 
	InputHandler::setupUI()
{
	//mTrayMgr->hideCursor();
	// make room for the controls
	//mTrayMgr->showLogo(OgreBites::TL_TOPRIGHT);
	mTrayMgr->createDecorWidget(OgreBites::TL_TOPRIGHT, "myLogo", "Examples/MyLogo");
	mTrayMgr->showFrameStats(OgreBites::TL_TOPRIGHT);
	mTrayMgr->toggleAdvancedFrameStats();

	mTrayMgr->createDecorWidget(OgreBites::TL_BOTTOM, "grass", "Examples/SelectGrass");

	

	


	// a friendly reminder
	StringVector names;
	names.push_back("Help");
	names.push_back("Exit");
	OgreBites::ParamsPanel* mDetailsPanel2 = mTrayMgr->createParamsPanel(OgreBites::TL_TOPLEFT, "Help", 100, names);
	mDetailsPanel2->setParamValue(0, "H/F1");
	mDetailsPanel2->setParamValue(1, "ESC");

	Ogre::StringVector items;
	items.push_back("Meter");
	items.push_back("Level");

	//mTrayMgr->createTextBox(OgreBites::TL_TOPLEFT,"camera","camera_y"+ Ogre::StringConverter::toString(OgreTrip::getCamera()->getPosition().y), 100,100);
	mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_TOPLEFT, "DetailsPanel", 200, items);
}




InputHandler *InputHandler::_instance = 0;
InputHandler*
	InputHandler::getSingleton()
{
	if (!_instance)
	{
		_instance = new InputHandler();
	}
	return _instance;
}


InputHandler::~InputHandler()
{
	//mInputManager->destroyInputObject(mCurrentKeyboard);
	//mInputManager->destroyInputObject(mCurrentMouse);
	mInputManager->destroyInputObject(mInputContext.mKeyboard);
	mInputManager->destroyInputObject(mInputContext.mMouse);
	OIS::InputManager::destroyInputSystem( mInputManager );
	DestroyInstance();
}


void
	InputHandler::DestroyInstance() {
		delete _instance;
		_instance = 0;
}

void InputHandler::gameOver()
{
	std::string bestScore;
	std::ifstream fil;
	fil.open("score.txt");
	fil>>bestScore;

	OgreTrip::getSoundEng()->play2D(loseSound.c_str());
	int bestScoreInt;//number which will contain the result
	stringstream convert(bestScore); // stringstream used for the conversion initialized with the contents of Text

	if ( !(convert >> bestScoreInt) )//give the value to Result using the characters in the string
		bestScoreInt = 0;//if that fails set Result to 0
	
	std::ofstream file;
	file.open("score.txt");
	if ((int)meter> bestScoreInt)
	{
		bestScoreInt = (int)meter;
		file<<(int)meter<<std::endl;
	}
	else file<<bestScoreInt<<std::endl;

	Ogre::String gameOverString = "Best Score:" + Ogre::StringConverter::toString( (int)bestScoreInt) + "\nCurrent Score: " + Ogre::StringConverter::toString( (int)meter);
	mTrayMgr->showOkDialog("Game Over", gameOverString);
	meter = 0;
	OgreTrip::level = 1;
	addPower = false;
	power = 300.0;

}

void InputHandler::buttonHit(OgreBites::Button *b)
{
	if(b->getName()=="StartButton")
	{
		OgreTrip::gameStart = true;
		helpCreated = false;
		OgreTrip::pause = false;
		mTrayMgr->destroyWidget(OgreBites::TL_CENTER,"StartHelp");
		mTrayMgr->destroyWidget(OgreBites::TL_CENTER,"StartButton");
	}
}

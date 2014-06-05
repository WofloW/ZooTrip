#ifndef __InputHandler_h_
#define __InputHandler_h_

#include <ois/ois.h>
#include <Ogre.h>
#include "OgreRenderWindow.h"
#include "Ogre.h"
#include "OgreStringConverter.h"
#include <stdio.h>
#include "SdkTrays.h"
#include "Global.h"
#include "AIManager.h"


class InputHandler  : public OIS::MouseListener, public OIS::KeyListener, public OgreBites::SdkTrayListener
{
public:

	static InputHandler *getSingleton();
	~InputHandler();
	void DestroyInstance();

	void Think(float time);
	bool IsKeyDown(OIS::KeyCode key);

	virtual bool mouseMoved( const OIS::MouseEvent& e );
	virtual bool mousePressed( const OIS::MouseEvent& e, OIS::MouseButtonID button );
	virtual bool mouseReleased( const OIS::MouseEvent& e, OIS::MouseButtonID button );

	virtual bool keyPressed( const OIS::KeyEvent& e );
	virtual bool keyReleased( const OIS::KeyEvent& e );

	virtual void buttonHit(OgreBites::Button *b);
	OgreBites::SdkTrayManager* getSdkTrayMgr() {return mTrayMgr;}
	void setupUI();
	void gameOver();
	

protected:
	InputHandler();
	
	OIS::InputManager* mInputManager;
	Ogre::RenderWindow *mRenderWindow;
	//OIS::Keyboard *mPreviousKeyboard;
	//OIS::Keyboard *mCurrentKeyboard;
	//OIS::Mouse *mCurrentMouse;
	char mOldKeys[256];
	static InputHandler *_instance;

	OgreBites::InputContext mInputContext;		// all OIS devices are here
	OgreBites::SdkTrayManager* mTrayMgr;     		// tray interface manager
	OgreBites::ParamsPanel* mDetailsPanel;   		// sample details panel
	Ogre::NameValuePairList mInfo;    // custom sample info
	Ogre::Real mTopSpeed;
	Ogre::Vector3 mVelocity;

	bool mGoingForward;
	bool mGoingBack;
	bool mGoingLeft;
	bool mGoingRight;
	float power;

	float unitPower;
	enum FoodName selectFood;
	bool addPower;
	Ogre::Real mousez;
	Ogre::Real meter;

	bool helpCreated;
	std::string throwSound;
	std::string loseSound;
	Ogre::BillboardSet * m_pBillboardSet;
	Ogre::Billboard *m_pBillboard_StrengthBar;
	Ogre::SceneNode *billNode;

	
};

#endif
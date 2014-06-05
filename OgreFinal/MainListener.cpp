#include "MainListener.h"
#include "InputHandler.h"
#include "World.h"
#include "OgreTrip.h"

MainListener::MainListener() 
{

}


// On every frame, call the appropriate managers
bool 
	MainListener::frameStarted(const Ogre::FrameEvent &evt)
{
	
	float time = evt.timeSinceLastFrame;
	if (time > 0.5)
	{
		time = 0.5;
	}

	
	InputHandler::getSingleton()->Think(time);
	Geometry::getSingleton()->Think(evt);
	if( !OgreTrip::pause)
	{
		World::getSingleton()->Think(time);
		AIManager::getSingleton()->Think(time);
	}
	

	// Call think methods on any other managers / etc you want to add

	bool keepGoing = true;

	// Ogre will shut down if a listener returns false.  We will shut everything down if
	// either the user presses escape or the main render window is closed.  Feel free to 
	// modify this as you like.
	if (InputHandler::getSingleton()->IsKeyDown(OIS::KC_ESCAPE) || OgreTrip::getWindow()->isClosed())
	{
		keepGoing = false;
	}

	return keepGoing;
}
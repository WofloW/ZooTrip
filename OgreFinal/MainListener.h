#ifndef __MainListener_h_
#define __MainListener_h_

#include "Ogre.h"
#include "OgreFrameListener.h"
#include <ois.h>



class MainListener : public Ogre::FrameListener
{
public:
	MainListener();
	bool frameStarted(const Ogre::FrameEvent &evt);


	
	
};

#endif
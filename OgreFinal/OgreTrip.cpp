#include "OgreTrip.h"


Ogre::SceneManager* OgreTrip::mSceneManager = 0;
Ogre::RenderWindow* OgreTrip::mWindow = 0;
Ogre::Root* OgreTrip::mRoot = 0;
Ogre::Camera* OgreTrip::mCamera = 0;
irrklang::ISoundEngine* OgreTrip::soundEngine = 0;
bool OgreTrip::pause = true;
bool OgreTrip::gameStart = false;
int OgreTrip::level = 1;

OgreTrip::OgreTrip()
{
    mTripFrameListener = 0;
    mRoot = 0;
    // Provide a nice cross platform solution for locating the configuration files
    // On windows files are searched for in the current working directory, on OS X however
    // you must provide the full path, the helper function macBundlePath does this for us.
	//  (Note:  This is not fully tested under IOS)
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    mResourcePath = macBundlePath() + "/Contents/Resources/";
#else
    mResourcePath = "";
#endif
}


/// Standard destructor
OgreTrip::~OgreTrip()
{
    if (mTripFrameListener)
        delete mTripFrameListener;
    if (mRoot)
        delete mRoot;
}



void
OgreTrip::createCamera()
{
	mCamera = mSceneManager->createCamera("Cam_1");
}




void 
OgreTrip::createViewports(void)
{
	    
        Ogre::Viewport* vp = mWindow->addViewport(mCamera);
        vp->setBackgroundColour(Ogre::ColourValue(0.9f, 0.9f, 0.9f));
        mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));    

}

// Here is where we set up all of the non-rendering stuff (our world, various managers, etc)
void 
OgreTrip::createScene() 
{
	std::string bgmfile = "zoo.mp3";
	soundEngine = irrklang::createIrrKlangDevice();
	irrklang::ISound* BGM = soundEngine->play2D(bgmfile.c_str(),true, false, true);

	InputHandler::getSingleton();
	Geometry::getSingleton();
	World::getSingleton();
	AIManager::getSingleton();


}

bool 
OgreTrip::setup(void)
{

    Ogre::String pluginsPath;
	Ogre::String configPath;
    // only use plugins.cfg if not static
#ifndef OGRE_STATIC_LIB

#if OGRE_DEBUG_MODE == 1 && (OGRE_PLATFORM != OGRE_PLATFORM_APPLE && OGRE_PLATFORM != OGRE_PLATFORM_APPLE_IOS)
	// add _d suffix to config files
	pluginsPath = mResourcePath + "plugins_d.cfg";
	configPath = mResourcePath + "ogre_d.cfg";
#else
	pluginsPath = mResourcePath + "plugins.cfg";
	configPath = mResourcePath + "ogre.cfg";

#endif


#endif
	
    mRoot = new Ogre::Root(pluginsPath, 
        configPath, mResourcePath + "Ogre.log");

    setupResources();

    bool carryOn = configure();
    if (!carryOn) return false;

    mSceneManager = mRoot->createSceneManager(Ogre::ST_GENERIC, "TripSMInstance");
	mSceneManager->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);
	

    createCamera();
    createViewports();

    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);


	translaterlaySystem = new Ogre::OverlaySystem();
	mSceneManager->addRenderQueueListener(translaterlaySystem);


    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    createScene();


    // When we start the renderer, it will run forever, making callbacks every
    // frame to the frame listeners.  These listeners are where all of the non-
    // rendering work is done.  
    createFrameListener();

    return true;

}

void 
OgreTrip::createFrameListener(void)
{
	mTripFrameListener = new MainListener();
	mRoot->addFrameListener(mTripFrameListener);
}


bool 
OgreTrip::configure(void)
{
	//For debug, no need to show config dialog
	//mRoot->restoreConfig();
	//mWindow = mRoot->initialise(true);
	//return true;
	//Normal process
	
    // Show the configuration dialog and initialise the system
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg
    if(mRoot->showConfigDialog())
    {
        // If returned true, user clicked OK so initialise
        // Here we choose to let the system create a default rendering window by passing 'true'
        mWindow = mRoot->initialise(true);
        return true;
    }
    else
    {
        return false;
    }
	
}

void
OgreTrip::setupResources(void)
{
    // Load resource paths from config file
    Ogre::ConfigFile cf;

	#if OGRE_DEBUG_MODE == 1 && (OGRE_PLATFORM != OGRE_PLATFORM_APPLE && OGRE_PLATFORM != OGRE_PLATFORM_APPLE_IOS)
			// add _d suffix to config files
    cf.load(mResourcePath + "resources_d.cfg");
#else
	cf.load(mResourcePath + "resources.cfg");
#endif
		

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
            // OS X does not set the working directory relative to the app,
            // In order to make things portable on OS X we need to provide
            // the loading with it's own bundle path location
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                String(macBundlePath() + "/" + archName), typeName, secName);
#else
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
#endif
        }
    }
}

void
OgreTrip::destroyScene()
{
	soundEngine->drop();
}

void 
OgreTrip::go(void)
{
    if (!setup())
        return;

    mRoot->startRendering();

    // clean up
    destroyScene();
}


#ifndef __OgreTrip_h_
#define __OgreTrip_h_

#include "World.h"
#include "InputHandler.h"
#include "MainListener.h"
#include "Geometry.h"
#include "AIManager.h"

#include "Ogre.h"
#include "OgreConfigFile.h"
#include "OgreFontManager.h"
#include "OgreOverlaySystem.h"
#include "irrKlang.h"



class OgreTrip :  public Ogre::WindowEventListener
{
public:
    OgreTrip();
    ~OgreTrip();

    // Do all the application setup
    bool setup(void);

    // Start to run
    void go(void);

	
	static Ogre::SceneManager* getSceneMgr() {return mSceneManager;}
	static Ogre::Camera* getCamera() {return mCamera;}
	static irrklang::ISoundEngine* getSoundEng() {return soundEngine;}
	static Ogre::Root* getRoot() {return mRoot;}
	static Ogre::RenderWindow* getWindow() {return mWindow;}
	static bool pause;
	static bool gameStart;
	static int level;
protected:

    ///////////////////////////////////////////////////
    /// The following methods are all called by the public
    ///   setup method, to handle various initialization tasks
    //////////////////////////////////////////////////

    //  Load all the requrired resources (Mostly reading the file paths
    //  of various resources from the .cfg files)
    void setupResources(void);

    // Invoke the startup window for all of the Ogre settings (resolution, etc)
    bool configure(void);

    // Create all of the required classes and do setup for all non-rendering tasks
    void createScene(void);

    // Free up all memory & resources created in createScene
    void destroyScene(void);

    // Create the rendering camera (separate from the game camera -- the game camera
    //   contains the logic of how the camera should be translated, the rendering camera
    //   is used by the rendering system to render the scene.  So, the game camera 
    //   decides where the camera should be, and then makes calls to the rendering camera
    //   to translate the camera
	void createCamera(void);

	void createViewports(void);

    // The FrameListener is what receives callbacks from the rendering loop to handle 
    //  game logic
	void createFrameListener(void);

	MainListener *mTripFrameListener;
    Ogre::String mResourcePath;
	Ogre::OverlaySystem *translaterlaySystem;
	

	// All the static variables
	static Ogre::RenderWindow* mWindow;
	static Ogre::Root *mRoot;
    static Ogre::Camera* mCamera;
    static Ogre::SceneManager* mSceneManager;
	static irrklang::ISoundEngine* soundEngine; 

    


};

#endif // #ifndef __OgreTrip_h_
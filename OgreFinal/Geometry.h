#ifndef __Geometry_h_
#define __Geometry_h_

#include "Ogre.h"
#include "OgreSceneNode.h"
#include "OgreTerrain.h"
#include "OgreTerrainGroup.h"
#include "OgreTerrainQuadTreeNode.h"
#include "OgreTerrainMaterialGeneratorA.h"
#include "OgreTerrainPagedWorldSection.h"
#include "OgreTerrainPaging.h"
#include "SdkTrays.h"

using namespace Ogre;



class Geometry
{

public:

	Geometry();
	~Geometry();
	void Think(const FrameEvent& evt);
	static Geometry *getSingleton();
	void setupView();
	void configureTerrainDefaults(Light* l);
	void DestroyInstance();
	void _shutdown();
	void initBlendMaps(Ogre::Terrain* terrain);
	TerrainGroup* getTerrainGroup() {return mTerrainGroup;}
	Ogre::Real getGroundY(Ogre::Real x, Ogre::Real z);

protected:

	static Geometry *_instance;
	TerrainGlobalOptions* mTerrainGlobals;
	TerrainGroup* mTerrainGroup;
	TerrainPaging* mTerrainPaging;
	PageManager* mPageManager;
	PagedWorld* mPagedWorld;
	TerrainPagedWorldSection* mTerrainPagedWorldSection;


	/// This class just pretends to provide procedural page content to avoid page loading
	class DummyPageProvider : public PageProvider
	{
	public:
		bool prepareProceduralPage(Page* page, PagedWorldSection* section) { return true; }
		bool loadProceduralPage(Page* page, PagedWorldSection* section) { return true; }
		bool unloadProceduralPage(Page* page, PagedWorldSection* section) { return true; }
		bool unprepareProceduralPage(Page* page, PagedWorldSection* section) { return true; }
	};
	DummyPageProvider mDummyPageProvider;

	class SimpleTerrainDefiner : public TerrainPagedWorldSection::TerrainDefiner
	{
	public:
		virtual void define(TerrainGroup* terrainGroup, long x, long y)
		{
			Image img;
			img.load("terrain.png", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			/*
			if (x % 2)
				img.flipAroundY();
			if (y % 2)
				img.flipAroundX();
				*/
			terrainGroup->defineTerrain(x, y, &img);
		}
	};

	Vector3 mTerrainPos;
	OgreBites::Label* mInfoLabel;

	OgreBites::SdkTrayManager* m_TrayMgr;     		// tray interface manager



};

#endif
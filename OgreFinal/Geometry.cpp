#include "Geometry.h"
#include "OgreTrip.h"

// max range for a int16
#define ENDLESS_PAGE_MIN_X (-0x7FFF)
#define ENDLESS_PAGE_MIN_Y (-0x7FFF)
#define ENDLESS_PAGE_MAX_X 0x7FFF
#define ENDLESS_PAGE_MAX_Y 0x7FFF

#define ENDLESS_TERRAIN_FILE_PREFIX String("EndlessWorldTerrain")
#define ENDLESS_TERRAIN_FILE_SUFFIX String("dat")
#define TERRAIN_WORLD_SIZE 12000.0f
#define TERRAIN_SIZE 513
#define HOLD_LOD_DISTANCE 3000.0

Geometry::Geometry()
	: mTerrainGroup(0)
	, mTerrainPaging(0)
	, mPageManager(0)
	, mPagedWorld(0)
	, mTerrainPagedWorldSection(0)
	, mTerrainPos(0, 0, 0) {
		
		setupView();
		m_TrayMgr = InputHandler::getSingleton()->getSdkTrayMgr();
		mInfoLabel = m_TrayMgr->createLabel(OgreBites::TL_TOP, "TInfo", "", 350);

		mTerrainGlobals = OGRE_NEW TerrainGlobalOptions();

		//setDragLook(true);

		MaterialManager::getSingleton().setDefaultTextureFiltering(TFO_ANISOTROPIC);
		MaterialManager::getSingleton().setDefaultAnisotropy(7);

		//OgreTrip::getSceneMgr()->setFog(FOG_LINEAR, ColourValue(0.7, 0.7, 0.8), 0, 4000, 10000);

		LogManager::getSingleton().setLogDetail(LL_BOREME);

		Vector3 lightdir(0.55, -0.3, 0.75);
		lightdir.normalise();

		Light * l = OgreTrip::getSceneMgr()->createLight("tstLight");
		l->setType(Light::LT_DIRECTIONAL);
		l->setDirection(lightdir);
		l->setDiffuseColour(ColourValue::White);
		l->setSpecularColour(ColourValue(0.4, 0.4, 0.4));

		OgreTrip::getSceneMgr()->setAmbientLight(ColourValue(0.5f, 0.5f, 0.5f));

		mTerrainGroup = OGRE_NEW TerrainGroup(OgreTrip::getSceneMgr(), Terrain::ALIGN_X_Z, TERRAIN_SIZE, TERRAIN_WORLD_SIZE);
		mTerrainGroup->setFilenameConvention(ENDLESS_TERRAIN_FILE_PREFIX, ENDLESS_TERRAIN_FILE_SUFFIX);
		mTerrainGroup->setOrigin(mTerrainPos);
		mTerrainGroup->setAutoUpdateLod( TerrainAutoUpdateLodFactory::getAutoUpdateLod(BY_DISTANCE) );

		configureTerrainDefaults(l);

		// Paging setup
		mPageManager = OGRE_NEW PageManager();
		// Since we're not loading any pages from .page files, we need a way just
		// to say we've loaded them without them actually being loaded
		mPageManager->setPageProvider( & mDummyPageProvider);
		mPageManager->addCamera(OgreTrip::getCamera());
		mPageManager->setDebugDisplayLevel(0);
		mTerrainPaging = OGRE_NEW TerrainPaging(mPageManager);
		mPagedWorld = mPageManager->createWorld();
		mTerrainPagedWorldSection = mTerrainPaging->createWorldSection(mPagedWorld, mTerrainGroup, 400, 500,
			ENDLESS_PAGE_MIN_X, ENDLESS_PAGE_MIN_Y,
			ENDLESS_PAGE_MAX_X, ENDLESS_PAGE_MAX_Y);

		//      mPerlinNoiseTerrainGenerator = OGRE_NEW PerlinNoiseTerrainGenerator;
		//      mTerrainPagedWorldSection->setDefiner( mPerlinNoiseTerrainGenerator );
		mTerrainPagedWorldSection->setDefiner( OGRE_NEW SimpleTerrainDefiner );
		TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
		while(ti.hasMoreElements())
		{
			Terrain* t = ti.getNext()->instance;
			initBlendMaps(t);
		}
		mTerrainGroup->freeTemporaryResources();

		OgreTrip::getSceneMgr()->setSkyBox(true, "Examples/CloudyNoonSkyBox");
		


}

void Geometry::initBlendMaps(Ogre::Terrain* terrain)
{
	Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
	Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
	Ogre::Real minHeight0 = 50; //0.175
	Ogre::Real fadeDist0 = 10;
	Ogre::Real minHeight1 = 50;
	Ogre::Real fadeDist1 = 15;
	float* pBlend0 = blendMap0->getBlendPointer();
	float* pBlend1 = blendMap1->getBlendPointer();
	for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
	{
		for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
		{
			Ogre::Real tx, ty;

			blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
			Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);
			Ogre::Real val = (height - minHeight0) / fadeDist0;
			val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
			*pBlend0++ = val;

			val = (height - minHeight1) / fadeDist1;
			val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
			*pBlend1++ = val;
		}
	}
	blendMap0->dirty();
	blendMap1->dirty();
	blendMap0->update();
	blendMap1->update();

}



void
	Geometry::setupView() {
		Ogre::Camera * mCamera = OgreTrip::getCamera();
		// put camera at world center, so that it's difficult to reach the edge
		Vector3 worldCenter(
			(ENDLESS_PAGE_MAX_X + ENDLESS_PAGE_MIN_X) / 2 * TERRAIN_WORLD_SIZE,
			0,
			-(ENDLESS_PAGE_MAX_Y + ENDLESS_PAGE_MIN_Y) / 2 * TERRAIN_WORLD_SIZE
			);
		//mCamera->setPosition(mTerrainPos + worldCenter);
		//mCamera->lookAt(mTerrainPos);
		mCamera->setNearClipDistance(0.1);
		mCamera->setFarClipDistance(8000);
		/*
		if (OgreTrip::getRoot()->getRenderSystem()->getCapabilities()->hasCapability(RSC_INFINITE_FAR_PLANE)) {
			mCamera->setFarClipDistance(0);   // enable infinite far clip distance if we can
		}*/
}

void
	Geometry::configureTerrainDefaults(Light * l) {
		// Configure global
		mTerrainGlobals->setMaxPixelError(8);
		// testing composite map
		mTerrainGlobals->setCompositeMapDistance(3000);
		//mTerrainGlobals->setUseRayBoxDistanceCalculation(true);
		mTerrainGlobals->getDefaultMaterialGenerator()->setLightmapEnabled(false);

		mTerrainGlobals->setCompositeMapAmbient(OgreTrip::getSceneMgr()->getAmbientLight());
		mTerrainGlobals->setCompositeMapDiffuse(l->getDiffuseColour());
		mTerrainGlobals->setLightMapDirection(l->getDerivedDirection());

		// Configure default import settings for if we use imported image
		Terrain::ImportData & defaultimp = mTerrainGroup->getDefaultImportSettings();
		defaultimp.terrainSize = TERRAIN_SIZE;
		defaultimp.worldSize = TERRAIN_WORLD_SIZE;
		defaultimp.inputScale = 600;
		defaultimp.minBatchSize = 33;
		defaultimp.maxBatchSize = 65;
		// textures
		/*
		defaultimp.layerList.resize(1);
		defaultimp.layerList[0].worldSize = 60;
		defaultimp.layerList[0].textureNames.push_back("grass.tga");
		*/
		defaultimp.layerList.resize(1);
		defaultimp.layerList[0].worldSize = 30;
		defaultimp.layerList[0].textureNames.push_back("grass_green-01_diffusespecular.dds");
		defaultimp.layerList[0].textureNames.push_back("grass_green-01_normalheight.dds");
		/*defaultimp.layerList[0].worldSize = 100;
		defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
		defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
		defaultimp.layerList[1].worldSize = 30;
		defaultimp.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
		defaultimp.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
		defaultimp.layerList[2].worldSize = 200;
		defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
		defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");
		/*
		defaultimp.layerList.resize(3);
		defaultimp.layerList[0].worldSize = 200;
		defaultimp.layerList[0].textureNames.push_back("SB_MossyD_s.tga");
		defaultimp.layerList[1].worldSize = 200;
		defaultimp.layerList[1].textureNames.push_back("DragonBlightPackIceB_s.tga");
		defaultimp.layerList[2].worldSize = 200;
		defaultimp.layerList[2].textureNames.push_back("DragonBlightSnowScrapeB_s.tga");
		*/

}



void
	Geometry::Think(const FrameEvent& evt) {
		
		InputHandler::getSingleton()->getSdkTrayMgr()->frameRenderingQueued(evt);
		
		if (mTerrainGroup->isDerivedDataUpdateInProgress()) {
			m_TrayMgr->moveWidgetToTray(mInfoLabel, OgreBites::TL_TOP, 0);
			mInfoLabel->show();
			mInfoLabel->setCaption("Building terrain...");
		} else {
			m_TrayMgr->removeWidgetFromTray(mInfoLabel);
			mInfoLabel->hide();
		}
		


		//mTerrainGroup->autoUpdateLodAll(false, Any( Real(HOLD_LOD_DISTANCE) ));
}

void
	Geometry::_shutdown() {
		if (mTerrainPaging) {
			OGRE_DELETE mTerrainPaging;
			mPageManager->destroyWorld( mPagedWorld );
			OGRE_DELETE mPageManager;
		}
		OGRE_DELETE mTerrainGlobals;



}

Geometry::~Geometry() {
	_shutdown();
	DestroyInstance();
}



Geometry * Geometry::_instance = 0;
Geometry *
	Geometry::getSingleton() {
		if (!_instance) {
			_instance = new Geometry();
		}
		return _instance;
}

void
	Geometry::DestroyInstance() {
		delete _instance;
		_instance = 0;
}

Ogre::Real Geometry::getGroundY(Ogre::Real x, Ogre::Real z)
{
	Ogre::Real ground_y = 0;
	Ray ray;
	ray.setOrigin(Vector3(x, 10000, z));
	ray.setDirection(Vector3::NEGATIVE_UNIT_Y);

	TerrainGroup::RayResult rayResult = mTerrainGroup->rayIntersects(ray);

	if (rayResult.hit)
		ground_y = rayResult.position.y;
	return ground_y;
}

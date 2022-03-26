#include "pch.h"
#include "MyEngine.h"

MyEngine::MyEngine() :
OgreBites::ApplicationContext("SC-T-637-GEDE Final Project")
{
}

void MyEngine::setup(void)
{
	OgreBites::ApplicationContext::setup();
	addInputListener(this);
	setupSceneManager();
	setupCamera();
	populateScene();
	setup_input_manager();
}

void MyEngine::populateScene()
{
	// Add Sinbad Model
	player_ = new PlayerAvatar(scene_manager_, "Sinbad.mesh");

	// Add Ground
	Plane plane(Ogre::Vector3::UNIT_Y, -5);
	MeshPtr groundMesh = MeshManager::getSingleton()
		.createPlane("MainGround", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			plane, 1000, 1000, 100, 100, true, 1, 50, 50, Ogre::Vector3::UNIT_Z);

	// build tangent vectors for our mesh, to show the normal texture
		// This will make the floor tiles look like they have depth, even thought they are flat
	unsigned short src, dest;
	if (!groundMesh->suggestTangentVectorBuildParams(VES_TANGENT, src, dest))
	{
		groundMesh->buildTangentVectors(VES_TANGENT, src, dest);
	}

	Entity* groundEntity = scene_manager_->createEntity("LightPlaneEntity", "MainGround");
	scene_manager_->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
	groundEntity->setCastShadows(false);

	// Specify the material that is going to be used to render the floor tiles
	groundEntity->setMaterialName("Custom/BrickTiles");

	// Set Shadow Technique
	scene_manager_->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_STENCIL_MODULATIVE);

	// Add Directional Light
	scene_manager_->setAmbientLight(Ogre::ColourValue(0.2f, 0.5f, 0.8f));
	Light* directionalLight = scene_manager_->createLight("DirectionalLight");
	directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
	directionalLight->setDiffuseColour(1, 1, 1);
	directionalLight->setSpecularColour(.4, .4, .4);
	SceneNode* directionalLightNode = scene_manager_->getRootSceneNode()->createChildSceneNode();
	directionalLightNode->attachObject(directionalLight);
	directionalLightNode->setDirection(Ogre::Vector3(0, -1, -1));

	GameObjectManager::initialize(
		scene_manager_,
		player_->getEntityNode()
	);

	const auto object = new StaticGameObject(
		scene_manager_,
		"MyCustomCube.mesh",
		Ogre::Vector3(-10, 4, 0),
		Ogre::Vector3(2, 10, 20)
	);
	GameObjectManager::add_static_game_object(object);
}

void MyEngine::setupCamera()
{
	// Add camera
	roaming_camera_ = new RoamingCamera(scene_manager_, getRenderWindow(), Vector3(0, 0, 50));
}

void MyEngine::setupSceneManager()
{
	// Get pointers to root and create scene manager
	root_ = getRoot();
	scene_manager_ = root_->createSceneManager();

	// Register scene with the RTSS
	RTShader::ShaderGenerator* shaderGenerator = RTShader::ShaderGenerator::getSingletonPtr();
	shaderGenerator->addSceneManager(scene_manager_);
}

void MyEngine::setup_input_manager()
{
	input_manager_ = new InputManager();
}


bool MyEngine::keyPressed(const OgreBites::KeyboardEvent& evt)
{
	if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)
		getRoot()->queueEndRendering();
	
	return true;
}

bool MyEngine::frameStarted(const Ogre::FrameEvent& evt)
{
	// Main "game loop" of the application
	// Let parent handle this callback as well
	OgreBites::ApplicationContext::frameStarted(evt);
	// Store the time that has passed since last time we got the callback
	const Ogre::Real delta_time = evt.timeSinceLastFrame;
	// Check what keys of the keyboard are being pressed
	const Uint8* state = SDL_GetKeyboardState(nullptr);

	// Update any subsystems
	if (input_manager_ != nullptr)
		input_manager_->update();

	if (roaming_camera_ != nullptr)
		roaming_camera_->update(
			delta_time, 
			input_manager_->get_camera_movement(), 
			player_->getEntityNode()->getPosition()
		);

	if (player_ != nullptr) 
		player_->update(
			delta_time, 
			roaming_camera_->get_direction(),
			input_manager_->get_character_movement()
		);

	// Update all the managed pickup objects
	PickupManager::Update(delta_time, state);

	return true;
}
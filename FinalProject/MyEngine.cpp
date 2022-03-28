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

	GameObjectManager::initialize(
		scene_manager_,
		player_
	);

	const auto ground_object = populate_ground();

	const auto object1 = new StaticGameObject(
		scene_manager_,
		"MyCustomCube.mesh",
		Ogre::Vector3(-10, 0, 0),
		Ogre::Vector3(1, 1, 1)
	);

	const auto object2 = new DynamicGameObject(
		scene_manager_,
		"MyCustomCube.mesh",
		Ogre::Vector3(-10, 0, -7),
		Ogre::Vector3(1, 1, 1),
		Ogre::Vector3(0, 0, 1),
		1
	);

	GameObjectManager::add_game_object(object1);
	GameObjectManager::add_game_object(object2);
	GameObjectManager::add_game_object(ground_object);


	// Set Shadow Technique
	scene_manager_->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_STENCIL_MODULATIVE);

	// Add Directional Light
	scene_manager_->setAmbientLight(Ogre::ColourValue(0.2f, 0.5f, 0.8f));
	Ogre::Light* directionalLight = scene_manager_->createLight("DirectionalLight");
	directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
	directionalLight->setDiffuseColour(1, 1, 1);
	directionalLight->setSpecularColour(.4, .4, .4);
	Ogre::SceneNode* directionalLightNode = scene_manager_->getRootSceneNode()->createChildSceneNode();
	directionalLightNode->attachObject(directionalLight);
	directionalLightNode->setDirection(Ogre::Vector3(0, -1, -1));
}

StaticGameObject* MyEngine::populate_ground() const
{
	// Add Ground
	const Ogre::Plane plane(Ogre::Vector3::UNIT_Y, -5);
	Ogre::MeshPtr groundMesh = Ogre::MeshManager::getSingleton()
		.createPlane("MainGround", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			plane, 1000, 1000, 100, 100, true, 1, 50, 50, Ogre::Vector3::UNIT_Z);

	// build tangent vectors for our mesh, to show the normal texture
		// This will make the floor tiles look like they have depth, even thought they are flat
	unsigned short src, dest;
	if (!groundMesh->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dest))
	{
		groundMesh->buildTangentVectors(Ogre::VES_TANGENT, src, dest);
	}

	Ogre::Entity* groundEntity = scene_manager_->createEntity("LightPlaneEntity", "MainGround");
	const auto ground_node = scene_manager_->getRootSceneNode()->createChildSceneNode();
	ground_node->attachObject(groundEntity);
	groundEntity->setCastShadows(false);

	// Specify the material that is going to be used to render the floor tiles
	groundEntity->setMaterialName("Custom/BrickTiles");

	return new StaticGameObject(ground_node, groundEntity);
}


void MyEngine::setupCamera()
{
	// Add camera
	roaming_camera_ = new RoamingCamera(scene_manager_, getRenderWindow(), Ogre::Vector3(0, 0, 50));
}

void MyEngine::setupSceneManager()
{
	// Get pointers to root and create scene manager
	root_ = getRoot();
	scene_manager_ = root_->createSceneManager();

	// Register scene with the RTSS
	Ogre::RTShader::ShaderGenerator* shaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
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

	// Update all the managed game objects
	GameObjectManager::update(delta_time, state);

	return true;
}
#include "pch.h"
#include "MyEngine.h"

MyEngine::MyEngine() : ApplicationContext("Ogre Hello World")
{
}

void MyEngine::setup()
{
	ApplicationContext::setup();

	Root* root = getRoot();
	SceneManager* sceneManager = root->createSceneManager();

	RTShader::ShaderGenerator* shaderGenerator = RTShader::ShaderGenerator::getSingletonPtr();
	shaderGenerator->addSceneManager(sceneManager);

	Light* light = sceneManager->createLight("MainLight");
	SceneNode* lightNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	lightNode->setPosition(0, 10, 15);
	lightNode->attachObject(light);

	SceneNode* cameraNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	cameraNode->setPosition(0, 0, 15);
	cameraNode->lookAt(Vector3(0, 0, -1), Node::TS_PARENT);
	Camera* cam = sceneManager->createCamera("MainCamera");
	cam->setNearClipDistance(5);
	cam->setAutoAspectRatio(true);
	cameraNode->attachObject(cam);
	getRenderWindow()->addViewport(cam);

	Entity* ent = sceneManager->createEntity("Sinbad.mesh");
	SceneNode* sinbadNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	sinbadNode->attachObject(ent);
}
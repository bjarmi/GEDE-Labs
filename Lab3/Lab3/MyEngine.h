#pragma once
#include "pch.h"
#include "PlayerAvatar.h"

using namespace Ogre;
using namespace OgreBites;

class MyEngine : public ApplicationContext, public InputListener
{
public:
	MyEngine();
	virtual ~MyEngine() {};
	void setup();
	bool keyPressed(const OgreBites::KeyboardEvent& evt);
	void setupSceneManager(void);
	void setupCamera();
	void populateScene();
	bool frameStarted(const Ogre::FrameEvent& evt);

private:
	Root* root_;
	SceneManager* scene_manager_;
	Camera* camera_;
	SceneNode* camera_node_;
	PlayerAvatar* player_;
};
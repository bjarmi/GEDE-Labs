#pragma once
#include "pch.h"
#include "PlayerAvatar.h"
#include "RoamingCamera.h"
#include "PickupManager.h"


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
	Ogre::Root* root_;
	Ogre::SceneManager* scene_manager_;
	RoamingCamera* roaming_camera_;
	PlayerAvatar* player_;
};
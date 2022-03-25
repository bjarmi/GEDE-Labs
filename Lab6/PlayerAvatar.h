#pragma once
#include "pch.h"

using namespace Ogre;

class PlayerAvatar 
{
public:
	PlayerAvatar(SceneManager* scene_manager, String mesh_file_name);
	virtual ~PlayerAvatar() = default;
	void update(Ogre::Real delta_time, const Uint8* state);
	void update(
		const Ogre::Real delta_time,
		const Ogre::Vector2 camera_direction,
		const Ogre::Vector2 character_movement
	);
	void move(
		const Ogre::Vector3 translate_vector,
		const float rotation, 
		const Ogre::Real delta_time
	) const;
	Radian GetRotation(const Ogre::Vector3& vec) const;
	void setIdleAnimationLoop();
	void setWalkingAnimationLoop();
	void StopAnimationLoop(void) const;
	void StartAnimationLoop(void) const;
	SceneNode* getEntityNode();

private:

	SceneManager* scene_manager_;
	Entity* entity_;
	SceneNode* entity_node_;
	AnimationState* animation_state_base_ = nullptr;
	AnimationState* animation_state_top_ = nullptr;

	Entity* right_sword;
	Entity* left_sword;


	float rotation_ = 0.0;
	float rotation_speed_ = 5.0f;
	float walking_speed_ = 5;
};


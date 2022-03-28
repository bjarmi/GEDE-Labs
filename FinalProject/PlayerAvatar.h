#pragma once
#include "pch.h"
#include "AABBCollider.h"


class PlayerAvatar 
{
public:
	PlayerAvatar(
		Ogre::SceneManager* scene_manager,
		Ogre::String mesh_file_name
	);
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
	Ogre::Radian GetRotation(const Ogre::Vector3& vec) const;
	void setIdleAnimationLoop();
	void setWalkingAnimationLoop();
	void StopAnimationLoop(void) const;
	void StartAnimationLoop(void) const;
	Ogre::SceneNode* getEntityNode();
	AABBCollider* get_collider() const;

private:

	Ogre::SceneManager* scene_manager_;
	Ogre::Entity* entity_;
	Ogre::SceneNode* entity_node_;
	Ogre::AnimationState* animation_state_base_ = nullptr;
	Ogre::AnimationState* animation_state_top_ = nullptr;

	AABBCollider* collider_;

	Ogre::Entity* right_sword;
	Ogre::Entity* left_sword;


	float rotation_ = 0.0;
	float rotation_speed_ = 5.0f;
	float walking_speed_ = 5;
};


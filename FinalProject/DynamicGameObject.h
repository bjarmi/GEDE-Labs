#pragma once
#include "pch.h"
#include "IGameObject.h"
#include "AABBCollider.h"


class DynamicGameObject : public IGameObject
{
public:
	DynamicGameObject(
		Ogre::SceneManager* scene_manager,
		const char* mesh_file_name,
		Ogre::Vector3 position = Ogre::Vector3(0, 0, 0),
		const Ogre::Vector3 scale = Ogre::Vector3(1, 1, 1),
		const Ogre::Vector3 direction = Ogre::Vector3(0, 0, 0),
		const float speed = 0,
		const float mass = 1
	);

	~DynamicGameObject() override;

	Ogre::SceneNode* get_scene_node() const override;
	Ogre::Entity* get_entity() const override;
	ICollider* get_collider() const override;

	void update(const Ogre::Real delta_time) override;

	void apply_force(
		const Ogre::Vector3 force, 
		const Ogre::Real delta_time) override;

	void collide(IGameObject* other_object) const override;

private:
	Ogre::SceneNode* entity_node_;
	Ogre::Entity* entity_;
	AABBCollider* collider_;
	Ogre::Vector3 velocity_;
	float mass_;
	float friction_;
	Ogre::Vector3 previous_position_;
};


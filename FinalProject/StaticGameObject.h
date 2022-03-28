#pragma once
#include "pch.h"
#include "IGameObject.h"
#include "AABBCollider.h"

class StaticGameObject : public IGameObject
{
public:
	StaticGameObject(
		Ogre::SceneManager* scene_manager,
		const char* mesh_file_name,
		const Ogre::Vector3 position = Ogre::Vector3(0, 0, 0),
		const Ogre::Vector3 scale = Ogre::Vector3(1, 1, 1)
	);

	StaticGameObject(Ogre::SceneNode* scene_node, Ogre::Entity* entity);

	~StaticGameObject() override;

	Ogre::SceneNode* get_scene_node() const override;
	Ogre::Entity* get_entity() const override;
	ICollider* get_collider() const override;

	void update(const Ogre::Real delta_time) override;

	void apply_force(
		const Ogre::Vector3 force,
		const Ogre::Real delta_time) override;

	void collide(IGameObject* other) const override;

private:
	Ogre::SceneNode* entity_node_;
	Ogre::Entity* entity_;
	AABBCollider* collider_;
};

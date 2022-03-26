#pragma once
#include "pch.h"
#include "IGameObject.h"
#include "ICollider.h"

class StaticGameObject : public IGameObject
{
public:
	StaticGameObject(
		Ogre::SceneManager* scene_manager,
		const char* mesh_file_name,
		const Ogre::Vector3 position = Ogre::Vector3(0, 0, 0),
		const Ogre::Vector3 scale = Ogre::Vector3(1, 1, 1)
	);
	~StaticGameObject() override;

	Ogre::SceneNode* get_scene_node() const override;
	Ogre::Entity* get_entity() const override;
	ICollider* get_collider() override;

	void update(Ogre::Real delta_time) const override;

private:
	Ogre::SceneManager* scene_manager_;
	Ogre::SceneNode* entity_node_;
	Ogre::Entity* entity_;
	ICollider* collider_ = nullptr;
};

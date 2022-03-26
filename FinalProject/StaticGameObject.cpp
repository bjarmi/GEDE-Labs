#include "pch.h"
#include "StaticGameObject.h"


StaticGameObject::StaticGameObject(
	Ogre::SceneManager* scene_manager, 
	const char* mesh_file_name, 
	const Ogre::Vector3 position, 
	const Ogre::Vector3 scale
)
{
	scene_manager_ = scene_manager;
	entity_ = scene_manager_->createEntity(mesh_file_name);
	entity_node_ = scene_manager_->getRootSceneNode()->createChildSceneNode();
	entity_node_->attachObject(entity_);
	entity_node_->setPosition(position);
	entity_node_->setScale(scale);
	entity_->setCastShadows(true);
}

StaticGameObject::~StaticGameObject()
{
	delete(collider_);
}

Ogre::Entity* StaticGameObject::get_entity() const
{
	return entity_;
}

Ogre::SceneNode* StaticGameObject::get_scene_node() const
{
	return entity_node_;
}

ICollider* StaticGameObject::get_collider()
{
	return collider_;
}

void StaticGameObject::update(Ogre::Real delta_time) const{}


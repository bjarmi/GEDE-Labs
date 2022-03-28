#include "pch.h"
#include "StaticGameObject.h"


StaticGameObject::StaticGameObject(
	Ogre::SceneManager* scene_manager, 
	const char* mesh_file_name, 
	const Ogre::Vector3 position, 
	const Ogre::Vector3 scale
)
{
	entity_ = scene_manager->createEntity(mesh_file_name);
	entity_node_ = scene_manager->getRootSceneNode()->createChildSceneNode();
	entity_node_->attachObject(entity_);
	entity_node_->setPosition(position);
	entity_node_->setScale(scale);
	entity_->setCastShadows(true);
	entity_node_->showBoundingBox(true);
	collider_= new AABBCollider(entity_node_);
}

StaticGameObject::StaticGameObject(Ogre::SceneNode* scene_node, Ogre::Entity* entity)
{
	entity_node_ = scene_node;
	entity_ = entity;
	entity_node_->showBoundingBox(true);
	collider_ = new AABBCollider(entity_node_);
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

ICollider* StaticGameObject::get_collider() const
{
	return collider_;
}

void StaticGameObject::update(const Ogre::Real delta_time)
{
	return;	
}

void StaticGameObject::apply_force(
	const Ogre::Vector3 force,
	const Ogre::Real delta_time
)
{
	return;
}

void StaticGameObject::collide(IGameObject* other) const
{
	return;
}

#include "pch.h"
#include "DynamicGameObject.h"


DynamicGameObject::DynamicGameObject(
	Ogre::SceneManager* scene_manager,
	const char* mesh_file_name,
	Ogre::Vector3 position,
	const Ogre::Vector3 scale,
	const Ogre::Vector3 direction,
	const float speed,
	const float mass
)
{
	entity_ = scene_manager->createEntity(mesh_file_name);
	entity_node_ = scene_manager->getRootSceneNode()->createChildSceneNode();
	entity_node_->attachObject(entity_);
	entity_node_->setPosition(position);
	entity_node_->setScale(scale);
	velocity_ = direction.normalisedCopy() * speed;
	mass_ = mass;
	previous_position_ = position;

	entity_->setCastShadows(true);
	entity_node_->showBoundingBox(true);
	collider_ = new AABBCollider(entity_node_);
}

DynamicGameObject::~DynamicGameObject()
{
	delete(collider_);
}

Ogre::Entity* DynamicGameObject::get_entity() const
{
	return entity_;
}

Ogre::SceneNode* DynamicGameObject::get_scene_node() const
{
	return entity_node_;
}

ICollider* DynamicGameObject::get_collider() const
{
	return collider_;
}

void DynamicGameObject::collide(IGameObject* other_object) const
{
	entity_node_->setPosition(previous_position_);
}

void DynamicGameObject::apply_force(
	const Ogre::Vector3 force,
	const Ogre::Real delta_time
)
{
	velocity_ += (force / mass_) * delta_time;
}


void DynamicGameObject::update(const Ogre::Real delta_time)
{
	previous_position_ = entity_node_->getPosition();
	const auto movement = velocity_;
	entity_node_->setPosition(previous_position_ + delta_time * movement);
}


#pragma once
#include "pch.h"
#include "ICollider.h"

class IGameObject
{
public:
	virtual ~IGameObject() = default;

	// Returns the SceneNode that holds this object
	virtual Ogre::SceneNode* get_scene_node() const = 0;
	// Returns the Entity attached to the object
	virtual Ogre::Entity* get_entity() const = 0;

	virtual ICollider* get_collider() const = 0;

	virtual void update(const Ogre::Real delta_time) = 0;

	virtual void apply_force(
		const Ogre::Vector3 force,
		const Ogre::Real delta_time) = 0;

	virtual void collide(IGameObject* other) const = 0;
};
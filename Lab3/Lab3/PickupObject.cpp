#include "pch.h"
#include "PickupObject.h"

PickupObject::PickupObject(Ogre::SceneManager* scene_manager, const char* mesh_file_name,
	Ogre::Vector3 position, Ogre::Vector3 scale)
{
	scene_manager_ = scene_manager;
	entity_ = scene_manager_->createEntity(mesh_file_name);
	entity_node_ = scene_manager_->getRootSceneNode()->createChildSceneNode();
	entity_node_->attachObject(entity_);
	entity_node_->setPosition(position);
	entity_node_->setScale(scale);
	entity_->setCastShadows(true);
	pickup_effect_ = nullptr;
	picked_up_ = false;
}

PickupObject::~PickupObject()
{
	if (pickup_effect_ != nullptr) delete(pickup_effect_);
}

Ogre::SceneNode* PickupObject::getSceneNode() const
{
	return  entity_node_;
}

IPickupEffect* PickupObject::getPickupEffect() const
{
	return pickup_effect_;
}

Ogre::Entity* PickupObject::getEntity() const
{
	return entity_;
}

bool PickupObject::isPickedUp() {
	return picked_up_;
}

bool PickupObject::collidesWith(Ogre::SceneNode* other_node)
{
	bool collision = false;
	// Calculate thier collision radius.

	Ogre::Vector3 this_scale = this->entity_node_->getScale();
	Ogre::Vector3 other_scale = other_node->getScale();

	float this_radius = std::max(this_scale.x, this_scale.z) / 2;
	float other_radius = std::max(other_scale.x, other_scale.z) / 2;

	// Get distance between nodes.

	float distance = this->entity_node_->getPosition().distance(other_node->getPosition());

	if (distance < (this_radius + other_radius))
		collision = true;

	return collision;
}

void PickupObject::runPickupEffect()
{
	// TODO: Instantiate and run the effect here (try velocity (5.0, -5, 50.0)
	
	picked_up_ = true;
}

void PickupObject::update(float delta_time) const
{
	if (pickup_effect_ != nullptr) {
		if (pickup_effect_->isRunning()) pickup_effect_->update(delta_time);
	}
}
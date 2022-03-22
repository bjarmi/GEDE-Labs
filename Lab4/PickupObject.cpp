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
	delete(pickup_effect_);
}

Ogre::SceneNode* PickupObject::getSceneNode() const
{
	return entity_node_;
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

	// Get distance between nodes.

	float distance = this->entity_node_->getPosition().distance(other_node->getPosition());

	if (distance < 5)  // TODO: Replace with sum og collision radius of both object.
		collision = true;

	return collision;
}

void PickupObject::runPickupEffect()
{
	pickup_effect_ = new SwirlEffect(this->entity_node_, Ogre::Vector3(5.0, -5, 50.0));
	picked_up_ = true;
}

void PickupObject::update(float delta_time) const
{
	if (pickup_effect_ != nullptr) {
		if (pickup_effect_->isRunning()) pickup_effect_->update(delta_time);
	}
}
#pragma once

#include "pch.h"
#include "IPickupEffect.h"


class IPickupObject
{
public:
	virtual ~IPickupObject() = default;

	virtual Ogre::SceneNode* getSceneNode() const = 0;				// Returns the scene node that holds this object
	virtual Ogre::Entity* getEntity() const = 0;                    // Returns the Entity attached to this object
	virtual IPickupEffect* getPickupEffect() const = 0;				// Returns the pickup effect attached to this object
	virtual bool isPickedUp() = 0;									// True if the pickup effect has been run
	
	virtual void runPickupEffect() = 0;								// Display a special motion effect on the game object
	virtual bool collidesWith(Ogre::SceneNode* other_node) = 0;		// True if the passed node is within distance of this object
	virtual void update(float delta_time) const = 0;				// Updates object, including any running motion effects
};

#pragma once
#include "ICollider.h"

class AABBCollider : public ICollider
{
public:
	AABBCollider(Ogre::SceneNode* entity);

	bool collides_with(const ICollider* other_collider) const;

	Ogre::AxisAlignedBox get_bounds() const override;

private:
	Ogre::SceneNode* object_;
};


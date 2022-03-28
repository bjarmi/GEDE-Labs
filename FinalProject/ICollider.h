#pragma once
#include "pch.h"

class ICollider
{
public:
	virtual ~ICollider() = default;

	virtual bool collides_with(const ICollider* other_collider) const = 0;

	virtual Ogre::AxisAlignedBox get_bounds() const = 0;
};
#pragma once
#include "pch.h"

class ICollider
{
public:
	virtual ~ICollider() = default;

	virtual bool collide(ICollider* other_collider) = 0;
};
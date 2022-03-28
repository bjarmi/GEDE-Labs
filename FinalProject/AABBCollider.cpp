#include "pch.h"
#include "AABBCollider.h"

AABBCollider::AABBCollider(Ogre::SceneNode* object)
{
	object_ = object;
}

bool AABBCollider::collides_with(const ICollider* other_collider) const
{
	const auto this_bounds = this->get_bounds();
	const auto other_bounds = other_collider->get_bounds();

	const auto this_min = this_bounds.getMinimum();
	const auto other_min = other_bounds.getMinimum();
	const auto this_max = this_bounds.getMaximum();
	const auto other_max = other_bounds.getMaximum();

	return (this_min.x <= other_max.x && this_max.x >= other_min.x) &&
		(this_min.y <= other_max.y && this_max.y >= other_min.y) &&
		(this_min.z <= other_max.z && this_max.z >= other_min.z);
}

Ogre::AxisAlignedBox AABBCollider::get_bounds() const
{
	const auto pos = object_->getPosition();
	const auto local_bounds = object_->getAttachedObject(0)->getBoundingBox();
	auto global_bounds = Ogre::AxisAlignedBox(
		local_bounds.getMinimum() + pos,
		local_bounds.getMaximum() + pos
	);
	global_bounds.scale(object_->getScale());

	return global_bounds;
}



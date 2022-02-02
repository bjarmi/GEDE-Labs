#include "pch.h"
#include "SwirlEffect.h"

SwirlEffect::SwirlEffect(Ogre::SceneNode* scene_node, Ogre::Vector3 cylindrical_velocity_vector)
{
	scene_node_ = scene_node;
	cylindrical_velocity_vector_ = cylindrical_velocity_vector;
}

Ogre::Vector3 SwirlEffect::getCartesian() const
{
	Ogre::Real hight = cylindrical_velocity_vector_.x;
	Ogre::Real radius = cylindrical_velocity_vector_.y;
	Ogre::Real angle = cylindrical_velocity_vector_.z;

	
	return Ogre::Vector3(
		Ogre::Math::Cos(angle) * radius,
		hight,
		Ogre::Math::Sin(angle) * radius
	);
}

void SwirlEffect::update(float delta_time)
{
	if (effect_running_)
	{

		if (current_cylindrical_pos_.x >= end_cylindrical_pos_.x)
		{
			effect_running_ = false;
			effect_finished_ = true;
		}
		else
		{
			current_cylindrical_pos_ = current_cylindrical_pos_ + delta_time * cylindrical_velocity_vector_;
			scene_node_->setPosition(getCartesian() + center_pos_);
		}
	}
}
#include "pch.h"
#include "SwirlEffect.h"

SwirlEffect::SwirlEffect(Ogre::SceneNode* scene_node, Ogre::Vector3 const cylindrical_velocity_vector)
{
	scene_node_ = scene_node;
	center_pos_ = scene_node_->getPosition();
	cylindrical_velocity_vector_ = cylindrical_velocity_vector;

	start_cylindrical_pos_ = Ogre::Vector3(
		center_pos_.y,
		center_pos_.x / Ogre::Math::Cos(cylindrical_velocity_vector_.y),
		cylindrical_velocity_vector_.z
	);
	current_cylindrical_pos_ = start_cylindrical_pos_;
	end_cylindrical_pos_ = Ogre::Vector3(5, 0, 0); 

	run();
}

Ogre::Vector3 SwirlEffect::get_cylindrical() const { return current_cylindrical_pos_; }

Ogre::Vector3 SwirlEffect::get_cartesian() const
{
	Ogre::Real const height = current_cylindrical_pos_.x;
	Ogre::Real const radius = current_cylindrical_pos_.y;
	Ogre::Real const angle = current_cylindrical_pos_.z;

	auto const cartesian_vector = Ogre::Vector3(
		Ogre::Math::Cos(angle) * radius,
		height,
		Ogre::Math::Sin(angle) * radius
	);

	return cartesian_vector;
}

void SwirlEffect::update(const float delta_time)
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
			scene_node_->setPosition(get_cartesian() + center_pos_);
		}
	}
}

void SwirlEffect::run() { effect_running_ = true; }

bool SwirlEffect::isRunning() { return effect_running_; }

bool SwirlEffect::isFinished() { return effect_finished_; }
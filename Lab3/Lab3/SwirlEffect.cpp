#include "pch.h"
#include "SwirlEffect.h"

SwirlEffect::SwirlEffect(Ogre::SceneNode* scene_node, Ogre::Vector3 cylindrical_velocity_vector)
{
	scene_node_ = scene_node;
	center_pos_ = scene_node_->getParent()->getPosition();

	cylindrical_velocity_vector_ = cylindrical_velocity_vector;

	start_cylindrical_pos_ = Ogre::Vector3(
		scene_node_->getPosition().y,
		scene_node_->getPosition().x / Ogre::Math::Cos(cylindrical_velocity_vector_.z),
		cylindrical_velocity_vector_.z
	);
	current_cylindrical_pos_ = start_cylindrical_pos_;
	end_cylindrical_pos_ = Ogre::Vector3(5, 0, 0);


	effect_finished_ = false;
	run();
}

Ogre::Vector3 SwirlEffect::getCylindrical() const { return current_cylindrical_pos_; }

Ogre::Vector3 SwirlEffect::getCartesian() const
{
	Ogre::Real hight = current_cylindrical_pos_.x;
	Ogre::Real radius = current_cylindrical_pos_.y;
	Ogre::Real angle = current_cylindrical_pos_.z;

	
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

void SwirlEffect::run() { effect_running_ = true; }

bool SwirlEffect::isRunning() { return effect_running_; }

bool SwirlEffect::isFinished() { return effect_finished_; }
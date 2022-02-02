#include "pch.h"
#include "SwirlEffect.h"

SwirlEffect::SwirlEffect(Ogre::SceneNode* scene_node, Ogre::Vector3 cylindrical_velocity_vector)
{
	scene_node_ = scene_node;
	cylindrical_velocity_vector_ = cylindrical_velocity_vector;
}

Ogre::Vector3 SwirlEffect::getCartesian()
{

}
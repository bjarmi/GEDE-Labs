#include "pch.h"
#include "PlayerAvatar.h"

PlayerAvatar::PlayerAvatar(SceneManager* scene_manager, String mesh_file_name)
{
	scene_manager_ = scene_manager;
	entity_ = scene_manager_->createEntity(mesh_file_name);
	entity_->setCastShadows(true);
	entity_node_ = scene_manager_->getRootSceneNode()->createChildSceneNode();
	entity_node_->attachObject(entity_);

	// Sinbad specific
	entity_->getSubEntity(1)->setMaterialName("Sinbad/Body2");
	right_sword = scene_manager_->createEntity("Sword.mesh");
	left_sword = scene_manager_->createEntity("Sword.mesh");

	entity_->attachObjectToBone("Sheath.R", right_sword);
	entity_->attachObjectToBone("Sheath.L", left_sword);
}

SceneNode* PlayerAvatar::getEntityNode()
{
	return entity_node_;
}

void PlayerAvatar::update(Ogre::Real delta_time, const Uint8* state)
{
	// Leave if the right mouse button is being pressed (different function)
	if ((SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_RMASK)) return;
	// Construct the movement
	Ogre::Vector3 translate_vector(0, 0, 0);
	Ogre::Vector3 dir(sin(rotation_), 0, cos(rotation_));

	if (state[SDL_SCANCODE_W]) {
		translate_vector = walking_speed_ * dir;
	}
	if (state[SDL_SCANCODE_S]) {
		translate_vector = walking_speed_ * -1 * dir;
	}
	if (state[SDL_SCANCODE_A]) {
		rotation_ += delta_time * rotation_speed_;
	}
	if (state[SDL_SCANCODE_D]) {
		rotation_ -= delta_time * rotation_speed_;
	}

	move(translate_vector, rotation_, delta_time);

	if (!translate_vector.isZeroLength())
	{
		setWalkingAnimationLoop();
	}
	else
	{
		setIdleAnimationLoop();
	}
	animation_state_base_->addTime(delta_time);
	animation_state_top_->addTime(delta_time);
}

void PlayerAvatar::update(
	const Ogre::Real delta_time, 
	const Ogre::Vector2 camera_direction,
	const Ogre::Vector2 character_movement
)
{
	Ogre::Vector3 translate(
		camera_direction.x * character_movement.y, 
		0,
		camera_direction.y * character_movement.y
	);
	translate = Ogre::Vector3(
		translate.x + camera_direction.y * character_movement.x, 
		0,
		translate.z - camera_direction.x * character_movement.x
	);
	translate = translate.normalisedCopy();
	translate = translate * walking_speed_;

	// Keep player facing away from the camera
	rotation_ = (GetRotation(
		Ogre::Vector3(
			camera_direction.x, 
			0,
			camera_direction.y
		)) + Ogre::Radian(Ogre::Math::PI)).valueRadians();

	move(translate, rotation_, delta_time);

	if (!translate.isZeroLength())
	{
		setWalkingAnimationLoop();
	}
	else
	{
		setIdleAnimationLoop();
	}
	animation_state_base_->addTime(delta_time);
	animation_state_top_->addTime(delta_time);
}

void PlayerAvatar::move(
	const Ogre::Vector3 translate_vector,
	const float rotation, 
	const Ogre::Real delta_time
) const
{
	entity_node_->translate(translate_vector * delta_time * walking_speed_);
	entity_node_->resetOrientation();
	entity_node_->yaw(Ogre::Radian(rotation));
}

Ogre::Radian PlayerAvatar::GetRotation(const Ogre::Vector3& vec) const
{
	Ogre::Vector3 v(vec);
	v.normalise();
	return((v.x <= 0 ? -1.0 : 1.0) * Ogre::Radian(acos(v.z)));
}

void PlayerAvatar::setWalkingAnimationLoop()
{
	if (animation_state_top_ == nullptr)
	{
		animation_state_base_ = entity_->getAnimationState("RunBase");
		animation_state_top_ = entity_->getAnimationState("RunTop");
		StartAnimationLoop();

	}
	// Stop other animation if there is any already running
	else if (animation_state_top_->getAnimationName() != "RunTop")
	{
		StopAnimationLoop();
		animation_state_base_ = entity_->getAnimationState("RunBase");
		animation_state_top_ = entity_->getAnimationState("RunTop");
		StartAnimationLoop();
	}
}

void PlayerAvatar::setIdleAnimationLoop()
{
	if (animation_state_top_ == nullptr)
	{
		animation_state_base_ = entity_->getAnimationState("IdleBase");
		animation_state_top_ = entity_->getAnimationState("IdleTop");
		StartAnimationLoop();

	}
	// Stop other animation if there is any already running
	else if (animation_state_top_->getAnimationName() != "IdleTop")
	{
		StopAnimationLoop();
		animation_state_base_ = entity_->getAnimationState("IdleBase");
		animation_state_top_ = entity_->getAnimationState("IdleTop");
		StartAnimationLoop();
	}
}

void PlayerAvatar::StopAnimationLoop(void) const
{
	if (animation_state_base_ != nullptr &&
		animation_state_base_->getEnabled())
	{
		animation_state_base_->setTimePosition(0.0f);
		animation_state_base_->setEnabled(false);
	}
	if (animation_state_top_ != nullptr &&
		animation_state_top_->getEnabled())
	{
		animation_state_top_->setTimePosition(0.0f);
		animation_state_top_->setEnabled(false);
	}
}

void PlayerAvatar::StartAnimationLoop(void) const
{
	if (animation_state_base_ != nullptr &&
		!animation_state_base_->getEnabled())
	{
		animation_state_base_->setLoop(true);
		animation_state_base_->setEnabled(true);
	}
	if (animation_state_top_ != nullptr &&
		!animation_state_top_->getEnabled())
	{
		animation_state_top_->setLoop(true);
		animation_state_top_->setEnabled(true);
	}
}
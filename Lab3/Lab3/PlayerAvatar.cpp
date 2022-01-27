#include "pch.h"
#include "PlayerAvatar.h"

PlayerAvatar::PlayerAvatar(SceneManager* scene_manager, String mesh_file_name)
{
	scene_manager_ = scene_manager;
	entity_ = scene_manager_->createEntity(mesh_file_name);
	entity_->setCastShadows(true);
	entity_node_ = scene_manager_->getRootSceneNode()->createChildSceneNode();
	entity_node_->attachObject(entity_);
	animation_state_top_ = nullptr;
	animation_state_base_ = nullptr;
}

SceneNode* PlayerAvatar::getEntityNode()
{
	return entity_node_;
}

void PlayerAvatar::Update(Ogre::Real delta_time)
{
	const Uint8* state = SDL_GetKeyboardState(NULL);

	Ogre::Vector3 translate_vector(0, 0, 0);
	if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W]) {
		translate_vector += Ogre::Vector3(0.0, 0.0, -0.5);
	}
	if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S]) {
		translate_vector += Ogre::Vector3(0.0, 0.0, 0.5);
	}
	if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A]) {
		translate_vector += Ogre::Vector3(-0.5, 0.0, 0.0);
	}
	if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]) {
		translate_vector += Ogre::Vector3(0.5, 0.0, 0.0);
	}

	const bool is_walking = translate_vector != Ogre::Vector3(0, 0, 0);

	if (is_walking)
	{
		Move(translate_vector, delta_time);
		SetRunningAnimationLoop();
	}
	else
	{
		SetIdleAnimationLoop();
	}
	animation_state_base_->addTime(delta_time);
	animation_state_top_->addTime(delta_time);
}

void PlayerAvatar::Move(Ogre::Vector3 translate_vector, Ogre::Real delta_time)
{
	entity_node_->translate(translate_vector);
	entity_node_->resetOrientation();
	// Rotate the player to face in the direction of the translation
	entity_node_->yaw(GetRotation(translate_vector));
}

Ogre::Radian PlayerAvatar::GetRotation(const Ogre::Vector3& vec)
{
	Ogre::Vector3 v(vec);
	v.normalise();
	return((v.x <= 0 ? -1.0 : 1.0) * Ogre::Radian(acos(v.z)));
}

void PlayerAvatar::SetRunningAnimationLoop()
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

void PlayerAvatar::SetIdleAnimationLoop()
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
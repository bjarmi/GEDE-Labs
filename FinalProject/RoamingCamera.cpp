#include "pch.h"
#include "RoamingCamera.h"
RoamingCamera::RoamingCamera(
	Ogre::SceneManager* scene_manager,
	Ogre::RenderWindow* render_window,
	const Ogre::Vector3 position,
	const Ogre::Vector3 look_at_position
)
{
	scene_manager_ = scene_manager;
	// Creating and attaching the camera
	camera_yaw_node_ = scene_manager->getRootSceneNode()->createChildSceneNode("camera_yaw_node_");
	camera_pitch_node_ = camera_yaw_node_->createChildSceneNode("camera_pitch_node_");

	camera_ = scene_manager->createCamera("RoamingCamera");
	camera_pitch_node_->attachObject(camera_);

	camera_yaw_node_->setPosition(position);
	camera_pitch_node_->setFixedYawAxis(true);
	camera_pitch_node_->lookAt(
		look_at_position, 
		Ogre::Node::TransformSpace::TS_WORLD
	);


	Ogre::ColourValue fadeColour(0.9, 0.9, 0.9);

	// Configuring the camera
	camera_->setNearClipDistance(5);
	Ogre::Viewport* vp = render_window->addViewport(camera_);
	vp->setBackgroundColour(fadeColour);
	camera_->setAspectRatio(
		Ogre::Real(
			vp->getActualWidth()) 
			/ Ogre::Real(vp -> getActualHeight()
			)
	);
}

void RoamingCamera::update(
	const Ogre::Real delta_time, 
	const Uint8* keyboard_state
)
{
	int x = 0, y = 0;
	// Leave if right mouse button is not being pressed
	// ...but also retrieve and store mouse movement
	if (!(SDL_GetRelativeMouseState(&x, &y) & SDL_BUTTON_RMASK)) return;
	// Construct displacement vector
	auto vec = Ogre::Vector3(0, 0, 0);

	if (keyboard_state[SDL_SCANCODE_W]) 
		vec = Ogre::Vector3(0, 0, -1);
	
	if (keyboard_state[SDL_SCANCODE_S]) 
		vec += Ogre::Vector3(0, 0, 1);
	
	if (keyboard_state[SDL_SCANCODE_A]) 
		vec += Ogre::Vector3(-1, 0, 0);
	
	if (keyboard_state[SDL_SCANCODE_D]) 
		vec += Ogre::Vector3(1, 0, 0);
	
	if (keyboard_state[SDL_SCANCODE_Q]) 
		vec += Ogre::Vector3(0, 1, 0);
	
	if (keyboard_state[SDL_SCANCODE_E]) 
		vec += Ogre::Vector3(0, -1, 0);
	
	// Construct view rotation
	const float rot_x = x * delta_time * -1 * rotation_speed_;
	const float rot_y = y * delta_time * -1 * rotation_speed_;

	// Update camera with new displacement and rotation
	camera_yaw_node_->yaw(Ogre::Radian(rot_x));
	camera_pitch_node_->pitch(Ogre::Radian(rot_y));

	const Ogre::Vector3 direction = (
		camera_yaw_node_->getOrientation() * camera_pitch_node_->getOrientation()
		) * vec;

	camera_yaw_node_->translate(delta_time * movement_speed_ * direction);
}

void RoamingCamera::update(
	const Ogre::Real delta_time, 
	const Ogre::Vector2 camera_movement, 
	const Ogre::Vector3 player_position
)
{
	// Update the camera angle based on the joystick axis input
	camera_horizontal_angle_ = delta_time * camera_movement.x + camera_horizontal_angle_;
	camera_vertical_angle_ = delta_time * camera_movement.y + camera_vertical_angle_;

	const float horizontal_distance =
		distance_from_player_ * Ogre::Math::Cos(camera_vertical_angle_);

	const float vertical_distance =
		distance_from_player_ * Ogre::Math::Sin(camera_vertical_angle_);

	// Compute the right offset that allows the camera to "orbit" around the player as they rotate
	const auto camera_offset = Ogre::Vector3(
		horizontal_distance * Ogre::Math::Cos(camera_horizontal_angle_),
		vertical_distance,
		horizontal_distance * Ogre::Math::Sin(camera_horizontal_angle_)
	);
	camera_yaw_node_->setPosition(player_position + camera_offset);

	// Rotate the camera "horizontally" to match the player orientation
	const float rot_x = camera_movement.x * delta_time * -1;
	camera_yaw_node_->yaw(Ogre::Radian(rot_x));

	// Rotate the camera "vertically" to look at the player
	camera_pitch_node_->lookAt(
		player_position,
		Ogre::Node::TransformSpace::TS_WORLD
	);
}

Ogre::Vector2 RoamingCamera::get_direction() const
{
	const auto direction = Ogre::Vector2(
		Ogre::Math::Cos(camera_horizontal_angle_),
		Ogre::Math::Sin(camera_horizontal_angle_)
	);
	return direction.normalisedCopy();
}
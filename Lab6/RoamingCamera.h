#pragma once
#include "pch.h"
// ROAMING CAMERA
// Simple camera that can be moved around the scene using the mouse and keyboard (WASD)
// Activate the camera motion mode by holding down the right mouse button
class RoamingCamera
{
public:
	// CONSTRUCTOR
	// scene_manager: The main scene manager
	// render_window: The window to which the camera is attached and will display within
	// position: The Initial position of the camera in world coordinates
	// lookat_position: A point in world coordinates that the camera is pointing at
	RoamingCamera(
		Ogre::SceneManager* scene_manager, 
		Ogre::RenderWindow* render_window,
		const Ogre::Vector3 position,
		const Ogre::Vector3 look_at_position = Ogre::Vector3(0, 0, 0)
	);
	~RoamingCamera() = default;
	// UPDATE
	// Updates position and rotation of the camera on time that has passed and any key presses
	// delta_time: Time since last update in seconds
	// state: Keyboard state, retrieved using SDL_GetKeyboardState
	void update(
		const Ogre::Real delta_time, 
		const Uint8* keyboard_state
	);

	void update(
		const Ogre::Real delta_time,
		const Ogre::Vector2 camera_movement,
		const Ogre::Vector3 player_position
	);
	Ogre::Vector2 get_direction() const;

private:
	Ogre::SceneManager* scene_manager_;	// The main scene manager
	Ogre::Camera* camera_;                // The camera itself
	Ogre::SceneNode* camera_yaw_node_;    // The scene node that holds the camera
	Ogre::SceneNode* camera_pitch_node_;    // The scene node that holds the camera

	float movement_speed_ = 7.0f;          // How fast the camera moves when WASD keys are pressed
	float rotation_speed_ = 0.8f;          // How fast the camera rotates when mouse is moved

	float distance_from_player_ = 30.0f;

	float camera_horizontal_angle_ = 0.0f;
	float camera_vertical_angle_ = 0.0f;
};
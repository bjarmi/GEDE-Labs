#pragma once
class InputManager
{
public:
	InputManager();

	~InputManager();

	Ogre::Vector2 get_camera_movement() const;

	Ogre::Vector2 get_character_movement() const;

	void update();

private:
	SDL_Joystick* joystick_ = nullptr;
	Sint16 player_axis_x_ = 0;
	Sint16 player_axis_y_ = 0;
	Sint16 camera_x_ = 0;
	Sint16 camera_y_ = 0;

};


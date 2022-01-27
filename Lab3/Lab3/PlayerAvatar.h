#include "pch.h"

using namespace Ogre;

class PlayerAvatar
{
public:
	PlayerAvatar(SceneManager* scene_manager, String mesh_file_name);
	virtual ~PlayerAvatar() {};
	void update(Ogre::Real delta_time, const Uint8* state);
	void Move(Ogre::Vector3 translate_vector, float rotation, Ogre::Real delta_time);
	Radian GetRotation(const Ogre::Vector3& vec);
	void setIdleAnimationLoop();
	void setWalkingAnimationLoop();
	void StopAnimationLoop(void) const;
	void StartAnimationLoop(void) const;
	SceneNode* getEntityNode();

private:
	SceneManager* scene_manager_;
	Entity* entity_;
	SceneNode* entity_node_;
	AnimationState* animation_state_base_;
	AnimationState* animation_state_top_;

	float rotation_;
	float rotation_speed_;
	float walking_speed_;
};


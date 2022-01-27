#include "pch.h"

using namespace Ogre;

class PlayerAvatar
{
public:
	PlayerAvatar(SceneManager* scene_manager, String mesh_file_name);
	virtual ~PlayerAvatar() {};
	void Update(Ogre::Real delta_time);
	void Move(Ogre::Vector3 translate_vector, Ogre::Real delta_time);
	Radian GetRotation(const Ogre::Vector3& vec);
	void SetIdleAnimationLoop();
	void SetRunningAnimationLoop();
	void StopAnimationLoop(void) const;
	void StartAnimationLoop(void) const;
	SceneNode* getEntityNode();

private:
	SceneManager* scene_manager_;
	Entity* entity_;
	SceneNode* entity_node_;
	AnimationState* animation_state_base_;
	AnimationState* animation_state_top_;
};


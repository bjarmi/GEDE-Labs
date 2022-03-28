#include "pch.h"
#include "GameObjectManager.h"

template<> GameObjectManager* Ogre::Singleton<GameObjectManager>::msSingleton = nullptr;

Ogre::SceneManager* GameObjectManager::scene_manager_;
PlayerAvatar* GameObjectManager::player_;
std::list<IGameObject*> GameObjectManager::game_objects_;
const auto gravity = Ogre::Vector3(0, -9.81f, 0);

bool GameObjectManager::initialize(
	Ogre::SceneManager* scene_manager,
	PlayerAvatar* player
)
{
	if (msSingleton == nullptr)
	{
		msSingleton = OGRE_NEW GameObjectManager;
		if (!msSingleton->_initialize(scene_manager, player))
		{
			OGRE_DELETE msSingleton;
			msSingleton = nullptr;
			return false;
		}
	}
	return true;
}

void GameObjectManager::destroy()
{
	if (msSingleton != nullptr)
	{
		msSingleton->_destroy();

		OGRE_DELETE msSingleton;
		msSingleton = nullptr;
	}
}

GameObjectManager& GameObjectManager::get_singleton()
{
	assert(msSingleton);
	return *msSingleton;
}

GameObjectManager* GameObjectManager::get_singleton_ptr()
{
	return msSingleton;
}

void GameObjectManager::add_game_object(IGameObject* object)
{
	game_objects_.push_back(object);
}

void GameObjectManager::update(
	const Ogre::Real delta_time,
	const Uint8* state
)
{
	// Check player to object collision.
	for (const auto object : game_objects_) {
		if (player_->get_collider()->collides_with(
			object->get_collider()
		))
			std::cout << "Player is colliding." << std::endl;
	}

	// Check object to object collision
	for (auto i = game_objects_.begin(); i != game_objects_.end();)
	{
		IGameObject* current_object = *i;

		current_object->apply_force(gravity, delta_time);
		current_object->update(delta_time);

		for (auto j = ++i; j != game_objects_.end();)
		{
			IGameObject* other_object = *j;
			if (current_object->get_collider()->collides_with(
					other_object->get_collider()
				))
			{
				current_object->collide(other_object);
				other_object->collide(current_object);
			}
			++j;
		}
	}

}

bool GameObjectManager::_initialize(
	Ogre::SceneManager* scene_manager,
	PlayerAvatar* player
)
{
	OGRE_LOCK_AUTO_MUTEX;
	scene_manager_ = scene_manager;
	player_ = player;

	return true;
}

void GameObjectManager::_destroy()
{
	OGRE_LOCK_AUTO_MUTEX;
	OGRE_DELETE scene_manager_;
	scene_manager_ = nullptr;
	OGRE_DELETE player_;
	player_ = nullptr;
	OGRE_DELETE& game_objects_;
}






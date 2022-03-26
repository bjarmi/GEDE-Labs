#include "pch.h"
#include "GameObjectManager.h"

template<> GameObjectManager* Ogre::Singleton<GameObjectManager>::msSingleton = nullptr;

Ogre::SceneManager* GameObjectManager::scene_manager_;
Ogre::SceneNode* GameObjectManager::player_node_;
std::list<IGameObject*> GameObjectManager::static_game_objects_;
std::list<IGameObject*> GameObjectManager::dynamic_game_objects_;

bool GameObjectManager::initialize(
	Ogre::SceneManager* scene_manager,
	Ogre::SceneNode* player_node
)
{
	if (msSingleton == nullptr)
	{
		msSingleton = OGRE_NEW GameObjectManager;
		if (!msSingleton->_initialize(scene_manager, player_node))
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

void GameObjectManager::add_static_game_object(IGameObject* object)
{
	static_game_objects_.push_back(object);
}

void GameObjectManager::update(
	const Ogre::Real delta_time,
	const Uint8* state
)
{
	for (const auto object : dynamic_game_objects_)
		object->update(delta_time);

	// TODO: Check collision.
}

bool GameObjectManager::_initialize(
	Ogre::SceneManager* scene_manager,
	Ogre::SceneNode* player_node
)
{
	OGRE_LOCK_AUTO_MUTEX;
	scene_manager_ = scene_manager;
	player_node_ = player_node;

	return true;
}

void GameObjectManager::_destroy()
{
	OGRE_LOCK_AUTO_MUTEX;
	OGRE_DELETE scene_manager_;
	scene_manager_ = nullptr;
	OGRE_DELETE player_node_;
	player_node_ = nullptr;
	OGRE_DELETE& static_game_objects_;
}






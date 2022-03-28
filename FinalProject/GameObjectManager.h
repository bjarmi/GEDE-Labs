#pragma once
#include "pch.h"
#include "OgreSingleton.h"
#include "IGameObject.h"
#include "PlayerAvatar.h"

class GameObjectManager : public Ogre::Singleton<GameObjectManager>
{
public:
	GameObjectManager(GameObjectManager const&) = delete;
	void operator=(GameObjectManager const&) = delete;

    static bool initialize(
        Ogre::SceneManager* scene_manager,
        PlayerAvatar* player
    );

    /**
    Destroy the Pickup Manager.
    */
    static void destroy();


    /** Override standard Singleton retrieval. */
    static GameObjectManager& get_singleton();


    /// @copydoc Singleton::getSingleton()
    static GameObjectManager* get_singleton_ptr();

    static void add_game_object(IGameObject* object);


    static void update(
        const Ogre::Real delta_time,
        const Uint8* state
    );


protected:
    static Ogre::SceneManager* scene_manager_;
    static PlayerAvatar* player_;
    static std::list<IGameObject*> game_objects_;
    

    /** Class default constructor */
    GameObjectManager() = default;

    /** Class destructor */
    ~GameObjectManager() = default;

    /** Initialize the pickup manager instance. */
    static bool _initialize(
        Ogre::SceneManager* scene_manager, 
        PlayerAvatar* player
    );

    /** Destory the pickup manager instance. */
    static void _destroy();
};


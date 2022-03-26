#pragma once
#include "pch.h"
#include "OgreSingleton.h"
#include "IGameObject.h"

class GameObjectManager : public Ogre::Singleton<GameObjectManager>
{
public:
	GameObjectManager(GameObjectManager const&) = delete;
	void operator=(GameObjectManager const&) = delete;

    static bool initialize(
        Ogre::SceneManager* scene_manager,
        Ogre::SceneNode* player_node
    );

    /**
    Destroy the Pickup Manager.
    */
    static void destroy();


    /** Override standard Singleton retrieval. */
    static GameObjectManager& get_singleton();


    /// @copydoc Singleton::getSingleton()
    static GameObjectManager* get_singleton_ptr();

    static void add_static_game_object(IGameObject* object);


    static void update(
        const Ogre::Real delta_time,
        const Uint8* state
    );


protected:
    static Ogre::SceneManager* scene_manager_;
    static Ogre::SceneNode* player_node_;
    static std::list<IGameObject*> static_game_objects_;
    static std::list<IGameObject*> dynamic_game_objects_;
    

    /** Class default constructor */
    GameObjectManager() = default;

    /** Class destructor */
    ~GameObjectManager() = default;

    /** Initialize the pickup manager instance. */
    static bool _initialize(
        Ogre::SceneManager* scene_manager, 
        Ogre::SceneNode* player_node
    );

    /** Destory the pickup manager instance. */
    static void _destroy();
};


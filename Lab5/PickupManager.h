#pragma once
#include "pch.h"
#include "OgreSingleton.h"
#include "PickupObject.h"
#include <iostream>
#include <random>


class PickupManager : public Ogre::Singleton<PickupManager>
{
    public:
        PickupManager(PickupManager const&) = delete;
        void operator=(PickupManager const&) = delete;
        /**
        Initialize the Pickup Manager.
        Return true upon success.
        */
        static bool initialize(Ogre::SceneManager* sceneManager, Ogre::SceneNode* playerNode);

        static void initialize_tower_parameters();
        /**
        Destroy the Pickup Manager.
        */
        static void destroy();


        /** Override standard Singleton retrieval. */
        static PickupManager& getSingleton();


        /// @copydoc Singleton::getSingleton()
        static PickupManager* getSingletonPtr();

        static void add_pickup_object(const char* mesh_file_name);

        static void add_pickup_object(const char* mesh_file_name, Ogre::Vector3 position);

        static void Update(Ogre::Real delta_time, const Uint8* state);

        static void create_pickup_tower();

    protected:
        static Ogre::SceneManager* scene_manager_;
        static Ogre::SceneNode* player_node_;
        static std::list<IPickupObject*> pickup_objects;

		// Variables for tower
		static bool create_pickup_tower_;
        static float tower_current_height_;
        static int tower_current_layers_;
        static int tower_length_;
        static int tower_width_;
        static float tower_spawn_period_;
        static float tower_spawn_timer_;
        static float tower_max_layers_;
        static int tower_instantiation_milliseconds_;

        /** Class default constructor */
        PickupManager();

        /** Class destructor */
        ~PickupManager();

        /** Initialize the pickup manager instance. */
        static bool _initialize(Ogre::SceneManager* sceneManager, Ogre::SceneNode* playerNode);

        /** Destory the pickup manager instance. */
        void _destroy();

        static void create_pickup_tower_layer();
		static void destroy_pickup_tower();
};


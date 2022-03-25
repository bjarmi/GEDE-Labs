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

        /**
        Destroy the Pickup Manager.
        */
        static void destroy();


        /** Override standard Singleton retrieval. */
        static PickupManager& getSingleton();


        /// @copydoc Singleton::getSingleton()
        static PickupManager* getSingletonPtr();

        static void addPickupObject(const char* mesh_file_name);


        static void Update(Ogre::Real delta_time, const Uint8* state);


    protected:
        static Ogre::SceneManager* scene_manager_;
        static Ogre::SceneNode* player_node_;
        static std::list<IPickupObject*> pickup_objects;

        /** Class default constructor */
        PickupManager();

        /** Class destructor */
        ~PickupManager();

        /** Initialize the pickup manager instance. */
        static bool _initialize(Ogre::SceneManager* sceneManager, Ogre::SceneNode* playerNode);

        /** Destory the pickup manager instance. */
        void _destroy();
};


#include "pch.h"
#include "PickupManager.h"

template<> PickupManager* Ogre::Singleton<PickupManager>::msSingleton = nullptr;

// need to declare the static variables, otherwise we get a link error
Ogre::SceneManager* PickupManager::scene_manager_;
Ogre::SceneNode* PickupManager::player_node_;
std::list<IPickupObject*> PickupManager::pickup_objects_;


bool PickupManager::initialize(
    Ogre::SceneManager* sceneManager, 
    Ogre::SceneNode* playerNode
)
{
    if (msSingleton == NULL)
    {
        msSingleton = OGRE_NEW PickupManager;
        if (false == msSingleton->_initialize(sceneManager, playerNode))
        {
            OGRE_DELETE msSingleton;
            msSingleton = NULL;
            return false;
        }
    }

    return true;
}

void PickupManager::destroy()
{
    if (msSingleton != NULL)
    {
        msSingleton->_destroy();

        OGRE_DELETE msSingleton;
        msSingleton = NULL;
    }
}

PickupManager& PickupManager::getSingleton()
{
	assert(msSingleton);
	return (*msSingleton);
}

PickupManager* PickupManager::getSingletonPtr()
{
	return msSingleton;
}

/** This function spawns a new pickup object at a random position near the player */
void PickupManager::addPickupObject(const char* mesh_file_name)
{
    std::random_device rd;                           // obtain a random number from hardware
    std::mt19937 gen(rd());                          // seed the generator
    const std::uniform_real_distribution<> distr(-50, 50); // define the range

    // Create a new pickup Object in a random position near the player
    const Ogre::Vector3 playerPosition = player_node_->getPosition();
    const Ogre::Real random_x_offset = distr(gen);
    const Ogre::Real random_z_offset = distr(gen);
    const auto new_spawn_position = Ogre::Vector3(playerPosition.x + random_x_offset, -3.0f, playerPosition.z + random_z_offset);
    const auto pickup_object = new PickupObject(
        scene_manager_,
        mesh_file_name,
        new_spawn_position,
        Ogre::Vector3(1, 1, 1)
    );

    // Set the color of the object
    // const Ogre::String MaterialName = pickup_object->getEntity()->getMesh()->getSubMesh(0)->getMaterialName();
    // const Ogre::MaterialPtr pMaterial = Ogre::MaterialManager::getSingleton().getByName(MaterialName);
    
    // constexpr float r = 255.0f / 255;
    // constexpr float g = 215.0f / 255;
    // constexpr float b = 0.0f / 255;

    // const Ogre::ColourValue* color = new Ogre::ColourValue(r, g, b, 1.0F);
    
    // pMaterial->setDiffuse(*color);
    // pMaterial->setAmbient(*color);
    // pMaterial->setSpecular(*color);


    // Insert the new Pickup Object in the list of managed objects
    pickup_objects_.push_back(pickup_object);
}

void PickupManager::Update(Ogre::Real delta_time, const Uint8* state)
{
    // Update all the managed pickup objects, and delete them if they finished the effect
    for (auto i = pickup_objects_.begin(); i != pickup_objects_.end();) {
        bool erased = false;
        IPickupObject* pickup_object = *i;

        pickup_object->update(delta_time);

        // Check for collision with a game object that has not yet been picked up
        if (pickup_object->collidesWith(player_node_))
        {
            scene_manager_->getRootSceneNode()->removeChild(pickup_object->getSceneNode());  // Remove pickup object from root node.
            player_node_->addChild(pickup_object->getSceneNode());  // Add pickup object to player.
            pickup_object->runPickupEffect();
        }

            // If the effect is finished we can dispose of the object
            if (pickup_object->isPickedUp() && pickup_object->getPickupEffect()->isFinished())
            {
                pickup_object->getSceneNode()->detachObject(pickup_object->getEntity());  // Detach entity form scene node.
                scene_manager_->destroyEntity(pickup_object->getEntity());
                pickup_objects_.erase(i++);
                continue;
            }
        ++i;
    }
}

bool PickupManager::_initialize(Ogre::SceneManager* sceneManager, Ogre::SceneNode* playerNode)
{
    OGRE_LOCK_AUTO_MUTEX;
    // Do here initialization stuff if needed
    scene_manager_ = sceneManager;
    player_node_ = playerNode;

    return true;
}

void PickupManager::_destroy()
{
    OGRE_LOCK_AUTO_MUTEX;
    // Do here destruction stuff if needed
    OGRE_DELETE scene_manager_;
    scene_manager_ = NULL;
    OGRE_DELETE player_node_;
    player_node_ = NULL;
    OGRE_DELETE &pickup_objects_;
}

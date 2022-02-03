#include "pch.h"
#include "PickupManager.h"

template<> PickupManager* Ogre::Singleton<PickupManager>::msSingleton = 0;

// need to declare the static variables, otherwise we get a link error
Ogre::SceneManager* PickupManager::scene_manager_;
Ogre::SceneNode* PickupManager::player_node_;
std::list<IPickupObject*> PickupManager::pickup_objects;

PickupManager::PickupManager(){ }

PickupManager::~PickupManager(){ }

bool PickupManager::initialize(Ogre::SceneManager* sceneManager, Ogre::SceneNode* playerNode)
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
    std::uniform_real_distribution<> distr(-50, 50); // define the range

    // Create a new pickup Object in a random position near the player
    Ogre::Vector3 playerPosition = player_node_->getPosition();
    Ogre::Real randomXOffset = distr(gen);
    Ogre::Real randomZOffset = distr(gen);
    Ogre::Vector3 newSpawnPosition = Ogre::Vector3(playerPosition.x + randomXOffset, -3.0f, playerPosition.z + randomZOffset);
    PickupObject* pickupObject = new PickupObject(scene_manager_, mesh_file_name, newSpawnPosition, Ogre::Vector3(0.01, 0.01, 0.01));
    pickupObject->getEntity()->getMesh()->getSubMesh(0)->getMaterialName();


    // Set the color of the object
    Ogre::String MaterialName = pickupObject->getEntity()->getMesh()->getSubMesh(0)->getMaterialName();
    Ogre::MaterialPtr pMaterial = Ogre::MaterialManager::getSingleton().getByName(MaterialName);
    
    float r = 255.0f / 255;
    float g = 215.0f / 255;
    float b = 0.0f / 255;

    Ogre::ColourValue* color = new Ogre::ColourValue(r, g, b, 1.0F);
    
    pMaterial->setDiffuse(*color);
    pMaterial->setAmbient(*color);
    pMaterial->setSpecular(*color);


    // Insert the new Pickup Object in the list of managed objects
    pickup_objects.push_back(pickupObject);
}

void PickupManager::Update(Ogre::Real delta_time, const Uint8* state)
{
    // Update all the managed pickup objects, and delete them if they finished the effect
    for (auto i = pickup_objects.begin(); i != pickup_objects.end();) {
        bool erased = false;
        IPickupObject* pickupObject = *i;

        pickupObject->update(delta_time);

        // Check for collision with a game object that has not yet been picked up
        if (pickupObject->collidesWith(player_node_))
        {
            // BONUS
            // TODO: Make the scene node of the cube a child of the player's scene node, and center it on the player


            pickupObject->runPickupEffect();
        }

        if (pickupObject->isPickedUp())
            // If the effect is finished we can dispose of the object
            if (pickupObject->getPickupEffect()->isFinished())
            {
               
                erased = true;
            }


        // Don't increase the counter if we have deleted an item, otherwise it throws an error
        if (!erased) ++i;
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
    OGRE_DELETE &pickup_objects;
}

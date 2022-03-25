#include "pch.h"
#include "PickupManager.h"

template<> PickupManager* Ogre::Singleton<PickupManager>::msSingleton = 0;

// need to declare the static variables, otherwise we get a link error
Ogre::SceneManager* PickupManager::scene_manager_;
Ogre::SceneNode* PickupManager::player_node_;
std::list<IPickupObject*> PickupManager::pickup_objects;

// Variables for tower
bool PickupManager::create_pickup_tower_;
float PickupManager::tower_current_height_;
int PickupManager::tower_current_layers_;
int PickupManager::tower_length_;
int PickupManager::tower_width_;
float PickupManager::tower_spawn_period_;
float PickupManager::tower_spawn_timer_;
float PickupManager::tower_max_layers_;
int PickupManager::tower_instantiation_milliseconds_;

PickupManager::PickupManager(){ }

PickupManager::~PickupManager(){ }

bool PickupManager::initialize(Ogre::SceneManager* sceneManager, Ogre::SceneNode* playerNode)
{
    if (msSingleton == nullptr)
    {
        msSingleton = OGRE_NEW PickupManager;
        if (false == msSingleton->_initialize(sceneManager, playerNode))
        {
            OGRE_DELETE msSingleton;
            msSingleton = nullptr;
            initialize_tower_parameters();
            return false;
        }
    }

    return true;
}

void PickupManager::initialize_tower_parameters()
{
    create_pickup_tower_ = false;
    tower_spawn_period_ = 1.0f;
    tower_spawn_timer_ = 0.0f;
    tower_width_ = 6;
    tower_length_ = 6;
    tower_current_height_ = 0.0f;
    tower_max_layers_ = 10.0f;
    tower_current_layers_ = 0.0f;
    tower_instantiation_milliseconds_ = 0;
}

void PickupManager::destroy()
{
    if (msSingleton != nullptr)
    {
        msSingleton->_destroy();

        OGRE_DELETE msSingleton;
        msSingleton = nullptr;
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
void PickupManager::add_pickup_object(const char* mesh_file_name)
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
    pickup_objects.push_back(pickup_object);
}

/*
 * Add a new pickup object at a given position.
 */
void PickupManager::add_pickup_object(
    const char* mesh_file_name,
    const Ogre::Vector3 position
)
{
    const auto start = std::chrono::high_resolution_clock::now();

    // instantiation WITH memory allocation testing feature
    const auto pickupObject = new PickupObject(
        2,
        scene_manager_,
        mesh_file_name, 
        position,
        Ogre::Vector3(1, 1, 1)
    );

    const auto end = std::chrono::high_resolution_clock::now();
    const int duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    tower_instantiation_milliseconds_ += duration;

    pickup_objects.push_back(pickupObject);
}

void PickupManager::create_pickup_tower()
{
    create_pickup_tower_ = true;
}


void PickupManager::create_pickup_tower_layer()
{
    const auto tower_spawn_position = Ogre::Vector3(
        10.0f, 
        tower_current_height_, 
        10.0f
    );

    constexpr float offset = 2.0f;

    for (int i = 0; i < tower_width_; i++)
    {
        for (int j = 0; j < tower_length_; j++)
        {
            const auto brick_spawn_position = Ogre::Vector3(
                tower_spawn_position.x + (i * offset),
                tower_spawn_position.y,
                tower_spawn_position.z + (j * offset)
            );
            add_pickup_object(
                "MyCustomCube.mesh",
                brick_spawn_position
            );
        }
    }

    tower_current_height_ += offset;
    tower_current_layers_++;
}

void PickupManager::destroy_pickup_tower()
{
    for (auto i = pickup_objects.begin(); i != pickup_objects.end();) {
        IPickupObject* pickupObject = *i;
        pickupObject->getSceneNode()->detachAllObjects();
        scene_manager_->destroyEntity(pickupObject->getEntity());
        scene_manager_->destroySceneNode(pickupObject->getSceneNode());

        delete pickupObject;
        pickupObject = nullptr;

        i = pickup_objects.erase(i);
    }
}

void PickupManager::Update(Ogre::Real delta_time, const Uint8* state)
{
    if (create_pickup_tower_ && (tower_current_layers_ < tower_max_layers_))
    {
        tower_spawn_timer_ += delta_time;
        if (tower_spawn_timer_ >= tower_spawn_period_)
        {
            create_pickup_tower_layer();
            tower_spawn_timer_ = 0.0f;
        }
    }
    else if (create_pickup_tower_ && (tower_current_layers_ >= tower_max_layers_))
    {
        std::cout << "Time to instantiate pickup objects in the tower: " << tower_instantiation_milliseconds_ << " (ms)" << std::endl;
        destroy_pickup_tower();
        initialize_tower_parameters();
        create_pickup_tower();
    }

    // Update all the managed pickup objects, and delete them if they finished the effect
    for (auto i = pickup_objects.begin(); i != pickup_objects.end();) {
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
                pickup_objects.erase(i++);
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
    OGRE_DELETE &pickup_objects;
}

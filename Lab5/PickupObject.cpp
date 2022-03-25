#include "pch.h"
#include "PickupObject.h"


void PickupObject::initialize(
	Ogre::SceneManager* scene_manager,
	const char* mesh_file_name,
	const Ogre::Vector3 position,
	const Ogre::Vector3 scale
)
{
	scene_manager_ = scene_manager;
	entity_ = scene_manager_->createEntity(mesh_file_name);
	entity_node_ = scene_manager_->getRootSceneNode()->createChildSceneNode();
	entity_node_->attachObject(entity_);
	entity_node_->setPosition(position);
	entity_node_->setScale(scale);
	entity_->setCastShadows(true);
	pickup_effect_ = nullptr;
	picked_up_ = false;
}

PickupObject::PickupObject(
	Ogre::SceneManager* scene_manager,
	const char* mesh_file_name,
	const Ogre::Vector3 position,
	const Ogre::Vector3 scale
)
{
	initialize(scene_manager, mesh_file_name, position, scale);
}

PickupObject::PickupObject(
	const short allocation_mode,
	Ogre::SceneManager* scene_manager,
	const char* mesh_file_name,
	const Ogre::Vector3	position,
	const Ogre::Vector3 scale
)
{
	initialize(scene_manager, mesh_file_name, position, scale);

	this->allocation_mode_ = allocation_mode;

	switch (this->allocation_mode_)
	{
	case 0:
	{
		// C++ NEW
		for (auto i = 0; i < array_size_; i++)
		{
			arr_[i] = new int;
		}
		break;
	}
	case 1:
	{
		// OGRE NEW
		for (auto i = 0; i < array_size_; i++)
		{
			arr_[i] = OGRE_NEW(int);
		}
		break;
	}
	case 2:
	{
		// STACK ALLOCATOR
		stack_allocator_ = new StackAllocator();
		stack_allocator_->init(array_size_ * sizeof(int));

		for (auto i = 0; i < array_size_; i++)
		{
			arr_[i] = (int*)stack_allocator_->allocate(sizeof(int));
		}
		break;
	}

	default:
		break;
	}
}

PickupObject::~PickupObject()
{
	delete(pickup_effect_);

	switch (this->allocation_mode_)
	{
	case 0:
	{
		// C++ DELETE
		for (auto i = 0; i < array_size_; i++)
		{
			if (arr_[i] != nullptr)
			{
				delete arr_[i];
				arr_[i] = nullptr;
			}
		}
		break;
	}
	case 1:
	{
		// OGRE DELETE
		for (auto i = 0; i < array_size_; i++)
		{
			if (arr_[i] != nullptr)
			{
				OGRE_DELETE(arr_[i]);
				arr_[i] = nullptr;
			}
		}
		break;
	}
	case 2:
	{
		// STACK ALLOCATOR DELETE
		stack_allocator_->reset();
		delete stack_allocator_;
		break;
	}

	default:
		break;
	}

}

Ogre::SceneNode* PickupObject::getSceneNode() const
{
	return entity_node_;
}

IPickupEffect* PickupObject::getPickupEffect() const
{
	return pickup_effect_;
}

Ogre::Entity* PickupObject::getEntity() const
{
	return entity_;
}

bool PickupObject::isPickedUp() {
	return picked_up_;
}

bool PickupObject::collidesWith(Ogre::SceneNode* other_node)
{
	bool collision = false;

	// Get distance between nodes.

	float distance = this->entity_node_->getPosition().distance(other_node->getPosition());

	if (distance < 5)  // TODO: Replace with sum og collision radius of both object.
		collision = true;

	return collision;
}

void PickupObject::runPickupEffect()
{
	pickup_effect_ = new SwirlEffect(this->entity_node_, Ogre::Vector3(5.0, -5, 50.0));
	picked_up_ = true;
}

void PickupObject::update(float delta_time) const
{
	if (pickup_effect_ != nullptr) {
		if (pickup_effect_->isRunning()) pickup_effect_->update(delta_time);
	}
}
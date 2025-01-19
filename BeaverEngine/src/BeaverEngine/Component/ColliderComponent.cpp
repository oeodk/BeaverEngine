#include "BeaverEngine/Component/ColliderComponent.h"
#include "BeaverEngine/System/ColliderSystem.h"
#include "BeaverEngine/Core/Entity.h"

namespace bv
{
	ColliderComponent::ColliderComponent()
	{
		ColliderSystem::getInstance().registerComponent(this);
	}

	ColliderComponent::~ColliderComponent()
	{
		ColliderSystem::getInstance().removeComponent(this);
	}

	void ColliderComponent::onCollision(const ColliderComponent& other)
	{
		if (collision_handler_ != nullptr)
		{
			collision_handler_->onCollision(other);
		}
	}

	void ColliderComponent::resolve()
	{
		Component* handler = owner().getComponent(handler_type_);
		if (handler != nullptr)
		{
			collision_handler_ = dynamic_cast<CollisionComponent*>(handler);
		}
	}
}
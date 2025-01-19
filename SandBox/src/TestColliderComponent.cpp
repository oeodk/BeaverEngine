#include "TestColliderComponent.h"
#include <BeaverEngine/Core/Entity.h>
#include <BeaverEngine/Component/SpriteComponent.h>

void sandbox::TestColliderComponent::updateLogic(const bv::Timing& timing)
{
	auto* sprite = owner().getComponent<bv::SpriteComponent>();

	if (collide_)
	{
		sprite->setColor({ 1,0,0,1 });
		collide_ = false;
	}
	else
	{
		sprite->setColor({ 1,1,1,1 });

	}

}

void sandbox::TestColliderComponent::onCollision(const bv::ColliderComponent& other)
{
	collide_ = true;
}

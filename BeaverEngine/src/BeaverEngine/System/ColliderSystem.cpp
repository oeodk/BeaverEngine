#include "BeaverEngine/System/ColliderSystem.h"
#include "BeaverEngine/Core/Entity.h"
namespace bv
{
	ColliderSystem& ColliderSystem::getInstance()
	{
		static ColliderSystem collider_system;
		return collider_system;
	}

	void ColliderSystem::iterate(const Timing& dt)
	{
        std::vector<ColliderComponent*> colliders_vector;
        colliders_vector.reserve(std::size(collider_components_));
        for (auto c : collider_components_)
        {
            if (c->enabled() && c->owner().active())
            {
                colliders_vector.push_back(c);
            }
        }
        std::vector<std::pair<ColliderComponent*,ColliderComponent*>> collisions;

        for (int i = 0; i < std::size(colliders_vector); i++)
        {
            auto c1 = colliders_vector[i];

            for (int j = i + 1; j < std::size(colliders_vector); j++)
            {
                auto c2 = colliders_vector[j];
                if (c1->getFlag() & c2->getMask())
                {
                    if (c1->collides(*c2))
                    {
                        collisions.emplace_back(c1, c2);
                    }
                }
            }
        }

        for (auto col : collisions)
        {
            col.first->onCollision(*col.second);
            col.second->onCollision(*col.first);
        }
	}

	void ColliderSystem::registerComponent(ColliderComponent* collider_component)
	{
		collider_components_.insert(collider_component);
	}
	void ColliderSystem::removeComponent(ColliderComponent* collider_component)
	{
		collider_components_.erase(collider_component);
	}
}
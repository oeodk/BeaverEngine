#include "BeaverEngine/System/ManagerSystem.h"

namespace bv
{
	ManagerSystem& ManagerSystem::getInstance()
	{
		static ManagerSystem manager_system;
		return manager_system;
	}

	void ManagerSystem::iterate(const Timing& dt)
	{
		for (auto& manager_component : manager_components_)
		{
			manager_component->update(dt);
		}
	}
	void ManagerSystem::registerComponent(ManagerComponent* manager_component)
	{
		manager_components_.insert(manager_component);
	}
	void ManagerSystem::removeComponent(ManagerComponent* manager_component)
	{
		manager_components_.erase(manager_component);
	}
}
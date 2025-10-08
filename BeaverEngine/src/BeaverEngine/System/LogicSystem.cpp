#include "BeaverEngine/System/LogicSystem.h"
#include "BeaverEngine/Core/Entity.h"
#include "BeaverEngine/Core/std.h"
namespace bv
{
	LogicSystem& LogicSystem::getInstance()
	{
		static LogicSystem logic_system;
		return logic_system;
	}

	void LogicSystem::iterate(const Timing& dt)
	{
		for (auto& logic_component : logic_components_)
		{
			if(logic_component->enabled() && logic_component->owner().active())
			{
				logic_component->updateLogic(dt);
			}
		}

		std::for_each(std::execution::par,
			parallelized_logic_components_.begin(), parallelized_logic_components_.end(),
			[&](LogicComponent* logic_component) {
				if (logic_component->enabled() && logic_component->owner().active())
				{
					logic_component->updateLogic(dt);
				}		
			});
	}
	void LogicSystem::registerComponent(LogicComponent* logic_component, bool parallelize)
	{
		if (parallelize)
		{
			parallelized_logic_components_.insert(logic_component);
		}
		else
		{
			logic_components_.insert(logic_component);
		}
	}

	void LogicSystem::removeComponent(LogicComponent* logic_component, bool parallelize)
	{
		if (parallelize)
		{
			parallelized_logic_components_.erase(logic_component);
		}
		else
		{
			logic_components_.erase(logic_component);
		}
	}
}

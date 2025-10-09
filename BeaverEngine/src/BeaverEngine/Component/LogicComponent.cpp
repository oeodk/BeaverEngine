#include "BeaverEngine/Component/LogicComponent.h"
#include "BeaverEngine/System/LogicSystem.h"

namespace bv
{
	LogicComponent::LogicComponent(bool parallelize)
	{
		LogicSystem::getInstance().registerComponent(this, parallelize);
		parallelize_ = parallelize;
	}

	LogicComponent::~LogicComponent()
	{
		LogicSystem::getInstance().removeComponent(this, parallelize_);
	}
}
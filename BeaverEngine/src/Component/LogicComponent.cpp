#include "BeaverEngine/Component/LogicComponent.h"
#include "BeaverEngine/System/LogicSystem.h"

namespace bv
{
	LogicComponent::LogicComponent()
	{
		LogicSystem::getInstance().registerComponent(this);
	}

	LogicComponent::~LogicComponent()
	{
		LogicSystem::getInstance().removeComponent(this);
	}
}
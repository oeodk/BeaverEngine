#include "BeaverEngine/Component/LogicComponent.h"
#include "BeaverEngine/System/LogicSystem.h"

namespace bv
{
	inline LogicComponent::LogicComponent()
	{
		LogicSystem::getInstance().registerComponent(this);
	}

	inline LogicComponent::~LogicComponent()
	{
		LogicSystem::getInstance().removeComponent(this);
	}
}
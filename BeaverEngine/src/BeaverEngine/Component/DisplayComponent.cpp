#include "BeaverEngine/Component/DisplayComponent.h"
#include "BeaverEngine/System/DisplaySystem.h"

namespace bv
{
	DisplayComponent::DisplayComponent()
	{
		DisplaySystem::getInstance().registerComponent(this);
	}
	DisplayComponent::~DisplayComponent()
	{
		DisplaySystem::getInstance().removeComponent(this);
	}
}
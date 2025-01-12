#include "BeaverEngine/Component/ManagerComponent.h"
#include "BeaverEngine/System/ManagerSystem.h"
#include "BeaverEngine/Core/Scene.h"
#include "BeaverEngine/Core/Entity.h"

namespace bv
{
	ManagerComponent::ManagerComponent(Entity& owner)
		: Component(owner) 
	{
		ManagerSystem::getInstance().registerComponent(this);
	}
	ManagerComponent::~ManagerComponent()
	{
		ManagerSystem::getInstance().removeComponent(this);
	}
	Component* ManagerComponent::getManagerInternal(std::string_view type)
	{
		return Scene::findObject("Managers")->getComponent(type);
	}
}
#include "BeaverEngine/Entity.h"
#include "BeaverEngine/System/EntitySystem.h"

namespace bv
{
	void Entity::addChild(std::string_view name, const EntityRef& child)
	{
		childrens_.push_back(child);
		children_by_name_[std::string{name}] = child;
		child->parent_ = weak_from_this();
	}

	void Entity::removeChild(EntityRef child)
	{
		childrens_.erase(std::remove(childrens_.begin(), childrens_.end(), child), childrens_.end());
		child->parent_.reset();
		EntitySystem::remove(child);
	}

	Component* Entity::addComponent(std::string_view type, const ComponentDescription& init_value)
	{
		auto result = components_.emplace(type, Component::Create(type, *this));
		auto iterator = result.first;
		Component * comp = dynamic_cast<Component*>(iterator->second.get());//{
		comp->setup(init_value);
		return comp;
	}

	void Entity::clear()
	{
		for (const auto& c : childrens_)
		{
			EntitySystem::remove(c);
		}

		children_by_name_.clear();
		childrens_.clear();
		components_.clear();
	}
}

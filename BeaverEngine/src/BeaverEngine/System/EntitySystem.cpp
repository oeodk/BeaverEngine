#include "BeaverEngine/system/EntitySystem.h"
#include "BeaverEngine/Core/Entity.h"
#include "BeaverEngine/Core/Scene.h"

namespace bv
{
	EntitySystem& EntitySystem::getInstance()
	{
		static EntitySystem entity_system;
		return entity_system;
	}

	void recursiveRemoveTag(const EntityRef& entity)
	{
		if (entity->getTag() != "")
		{
			Scene::removeEntityTag(entity, entity->getTag());
		}
		for (auto& child : entity->getChildren())
		{
			recursiveRemoveTag(child);
		}
	}

	void EntitySystem::remove(const EntityRef& entity)
	{
		recursiveRemoveTag(entity);
		
		entity->setActive(false);
		getInstance().entity_to_remove_.push_back(entity);
	}
}

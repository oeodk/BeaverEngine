#include "BeaverEngine/system/EntitySystem.h"
#include "BeaverEngine/Entity.h"
namespace bv
{
	EntitySystem& EntitySystem::getInstance()
	{
		static EntitySystem entity_system;
		return entity_system;
	}

	void EntitySystem::remove(const EntityRef& entity)
	{
		entity->setActive(false);
		getInstance().entity_to_remove_.push_back(entity);
	}
}

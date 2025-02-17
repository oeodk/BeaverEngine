#pragma once
#include "BeaverEngine/Core/std.h"
#include "BeaverEngine/Core/TypeDef.h"

namespace bv::Scene
{
	// name : the entity name (ex : player, player1, ...)
	EntityRef CreateChild(const Description& descr, const EntityRef& parent);
	EntityRef addChildToRoot(const Description& descr);
	EntityRef findObject(std::string_view name);
	void removeEntityTag(const EntityRef& entity, const std::string& tag);

	//return the first entity found with this tag 
	Entity* getEntityWithTag(std::string_view tag);
	std::vector<Entity*>* getEntitiesWithTag(std::string_view tag);


	void load(const Description& descr);
	void clear();
}
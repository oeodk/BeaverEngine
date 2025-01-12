#pragma once
#include "BeaverEngine/Core/std.h"
#include "BeaverEngine/Core/TypeDef.h"

namespace bv::Scene
{
	// name : the entity name (ex : player, player1, ...)
	EntityRef CreateChild(const Description& descr, const EntityRef& parent);
	EntityRef addChildToRoot(const Description& descr);
	EntityRef findObject(std::string_view name);
	void load(const Description& descr);
	void clear();
}
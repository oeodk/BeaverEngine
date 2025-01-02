#pragma once
#include <BeaverEngine/std.h>

namespace YAML
{
	class Node;
}

namespace bv
{
	class Entity;
	typedef std::shared_ptr<Entity> EntityRef;


	typedef YAML::Node Description;
}
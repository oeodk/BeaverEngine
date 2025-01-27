#pragma once
#include <BeaverEngine/Core/std.h>

namespace YAML
{
	class Node;
}

namespace bv
{
	class Entity;
	typedef std::shared_ptr<Entity> EntityRef;

	typedef YAML::Node Description;
	namespace Descr
	{
		bv::Description load(const std::string& path);
	}
}
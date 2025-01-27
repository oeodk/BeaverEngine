#include "BeaverEngine/Core/TypeDef.h"
#include "yaml-cpp/yaml.h"

namespace bv
{
	Description bv::Descr::load(const std::string& path)
	{
		return YAML::LoadFile(path);
	}
}

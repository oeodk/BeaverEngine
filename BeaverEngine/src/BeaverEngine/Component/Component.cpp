#include "BeaverEngine/Component/Component.h"

namespace bv
{
	void Component::Register(std::string_view type, const CreateFnType& create_function)
	{
		component_registry_.emplace(type, create_function);
	}
	std::unique_ptr<Component> Component::Create(std::string_view type, Entity& parent)
	{
		const auto& create_function = component_registry_.at((std::string)type);

		return create_function(parent);
	}
}

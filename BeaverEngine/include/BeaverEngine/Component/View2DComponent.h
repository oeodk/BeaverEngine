#pragma once
#include "BeaverEngine/Component/Component.h"
#include "BeaverEngine/System/ViewSystem.h"
#include "BeaverEngine/Utils/View2D.h"
namespace bv
{
	class View2DComponent
		: public Component
	{
	public:
		static constexpr auto type_ = "View2D";

		View2DComponent(Entity& owner) : Component(owner) {}
		View2DComponent(const View2DComponent&) = default;
		View2DComponent& operator=(const View2DComponent&) = default;
		View2DComponent(View2DComponent&&) = default;
		View2DComponent& operator=(View2DComponent&&) = default;

		void setup(const ComponentDescription& init_value) override
		{

			View2D* view = ViewSystem::getInstance().addView(init_value.parameters.at("name").as<std::string>());
			for (const auto& view_parameter : init_value.parameters)
			{
				if(view_parameter.first != "name")
				{
					view->setupParameter(view_parameter.first, view_parameter.second);
				}
			}
		}
	};
}

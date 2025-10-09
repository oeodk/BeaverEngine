#pragma once
#include "BeaverEngine/Component/Component.h"
#include "BeaverEngine/System/WindowSystem.h"
#include "BeaverEngine/Utils/Window.h"

#include <glm/glm.hpp>
namespace bv
{
	class WindowComponent
		: public Component
	{
	public :
		static constexpr auto type_ = "Window";

		WindowComponent(Entity& owner) : Component(owner) {}
		WindowComponent(const WindowComponent&) = default;
		WindowComponent& operator=(const WindowComponent&) = default;
		WindowComponent(WindowComponent&&) = default;
		WindowComponent& operator=(WindowComponent&&) = default;

		void setup(const ComponentDescription& init_value) override
		{
            WindowProperties props;
			if (init_value.parameters.contains("bgColor"))
			{
				props.color = glm::vec4(
					init_value.parameters.at("bgColor")[0].as<float>(),
					init_value.parameters.at("bgColor")[1].as<float>(),
					init_value.parameters.at("bgColor")[2].as<float>(),
					init_value.parameters.at("bgColor")[3].as<float>());
			}
			else
			{
				props.color = glm::vec4(0, 0, 0, 1);
			}
			props.resizable = true;
			props.decorate = true;
			if (init_value.parameters.contains("resizable"))
			{
				props.resizable = init_value.parameters.at("resizable").as<bool>();
			}
			if (init_value.parameters.contains("decorate"))
			{
				props.decorate = init_value.parameters.at("decorate").as<bool>();
			}
			if (init_value.parameters.contains("position"))
			{
				props.position.x = init_value.parameters.at("position")[0].as<int>();
				props.position.y = init_value.parameters.at("position")[1].as<int>();
			}
			if (init_value.parameters.contains("vsync"))
			{
				props.vsync = init_value.parameters.at("vsync").as<bool>();
			}


            props.name = init_value.parameters.at("name").as<std::string>();
            props.title = init_value.parameters.at("title").as<std::string>();
            props.width = init_value.parameters.at("size")[0].as<unsigned int>();
            props.height = init_value.parameters.at("size")[1].as<unsigned int>();
			window_ = WindowSystem::getInstance().addWindow(init_value.parameters.at("name").as<std::string>(), props);
		}

		std::shared_ptr<Window> getWindow() const { return window_.lock(); }
	private:
		std::weak_ptr<Window> window_{};
	};
}

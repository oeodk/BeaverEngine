#pragma once
#include "BeaverEngine/Component/ManagerComponent.h"
#include "BeaverEngine/Utils/Window.h"
#include <glm/glm.hpp>

namespace bv
{
    class WindowManagerComponent :
        public ManagerComponent
    {
    public:
        static constexpr auto type_ = "WindowManager";

        WindowManagerComponent(Entity& owner) : ManagerComponent(owner) {}
        void setup(const ComponentDescription& init_value) override
        {
            for (const auto& window_data : init_value.parameters)
            {
                glm::vec4 color(0, 0, 0, 1);
                if (window_data.second.size() > 2)
                {
                    color.r = window_data.second[3].as<float>();
                    color.g = window_data.second[4].as<float>();
                    color.b = window_data.second[5].as<float>();
                    color.a = window_data.second[6].as<float>();
                }

                Window* shared = nullptr;
                if (!main_window_.expired())
                {
                    shared = main_window_.lock().get();;
                }

                addWindow(window_data.first, { window_data.first, window_data.second[0].as<std::string>(), window_data.second[1].as<unsigned int>(), window_data.second[2].as<unsigned int>(), color, shared });
            }
        }

        std::weak_ptr<Window> getWindow(std::string_view window_name) const
        {
            auto window = windows_.find((std::string)window_name);
            if (window == windows_.end())
            {
                return std::weak_ptr<Window>();
            }
            return window->second;
        }

        void addWindow(std::string_view window_name, const WindowProperties& properties)
        {
            const std::string window_n = std::string(window_name);
            windows_.insert({ window_n, Window::create(properties) });
            if (main_window_.expired())
            {
                main_window_ = windows_.at(window_n);
                base_window_size_.x = properties.width;
                base_window_size_.y = properties.height;
            }
        }
        void update(const Timing& dt) override;

        std::weak_ptr<Window> getMainWindow() const { return main_window_; }

        static const glm::vec2& getBaseWindowSize() { return base_window_size_; }

        void closeWindow(std::string_view window_name);
    private:
        std::unordered_map<std::string, std::shared_ptr<Window>> windows_;
        inline static glm::vec2 base_window_size_{};

        std::weak_ptr<Window> main_window_;

        // Hérité via ManagerComponent
    };
}


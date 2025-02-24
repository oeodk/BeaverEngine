#pragma once
#include "BeaverEngine/System/System.h"
#include "BeaverEngine/Utils/Window.h"
#include <glm/glm.hpp>

namespace bv
{
    class WindowSystem :
        public System
    {
    public:
        WindowSystem() = default;
        WindowSystem(const WindowSystem&) = default;
        WindowSystem(WindowSystem&&) = default;
        WindowSystem& operator=(const WindowSystem&) = default;
        WindowSystem& operator=(WindowSystem&&) = default;

        ~WindowSystem() = default;

        static WindowSystem& getInstance();

        void iterate(const Timing& dt) override;

        std::weak_ptr<Window> getWindow(std::string_view window_name) const
        {
            auto window = windows_.find((std::string)window_name);
            if (window == windows_.end())
            {
                return std::weak_ptr<Window>();
            }
            return window->second;
        }

        std::weak_ptr<Window> addWindow(std::string_view window_name, const WindowProperties& properties)
        {
            WindowProperties props = properties;
            Window* shared = nullptr;
            if (!main_window_.expired())
            {
                props.shared = main_window_.lock().get();
            }

            const std::string window_n = std::string(window_name);
            windows_.insert({ window_n, Window::create(props) });
            if (main_window_.expired())
            {
                main_window_ = windows_.at(window_n);
                main_window_id_ = window_name;
                base_window_size_.x = props.width;
                base_window_size_.y = props.height;
            }
            return windows_.at(window_n);
        }

        std::weak_ptr<Window> getMainWindow() const { return main_window_; }
        const std::string& getMainWindowId() const { return main_window_id_; }

        static const glm::vec2& getBaseWindowSize() { return base_window_size_; }

        void closeWindow(std::string_view window_name);

        glm::uvec2 getMonitorSize(unsigned int monitor_index = 0);
        unsigned int getMonitorCount();
    private:
        std::unordered_map<std::string, std::shared_ptr<Window>> windows_;
        inline static glm::vec2 base_window_size_{};

        std::weak_ptr<Window> main_window_;
        std::string main_window_id_;
    };
}


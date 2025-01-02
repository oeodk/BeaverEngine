#pragma once
#include "BeaverEngine/Window.h"
#include "GLFW/glfw3.h"

namespace bv
{
    class WindowsWindow :
        public Window
    {
    public :
        WindowsWindow(const WindowProperties& props);
        WindowsWindow(const WindowsWindow&) = default;
        WindowsWindow(WindowsWindow&&) = default;
        WindowsWindow& operator=(const WindowsWindow&) = default;
        WindowsWindow& operator=(WindowsWindow&&) = default;

        virtual ~WindowsWindow();

        void onUpdate() override;

        unsigned int getWidth() const override { return width_; }
        unsigned int getHeight() const override { return height_; }

        void setVSync(bool enabled) override;
        bool isVSync() const override{ return v_sync_; }
    private:
        void init(const WindowProperties& props);
        void shutdown();
    private :
        std::string title_;
        unsigned int width_;
        unsigned int height_;

        bool v_sync_;

        GLFWwindow* window_;
    };
}


#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "BeaverEngine/Utils/Window.h"

namespace bv
{
    class DesktopWindow :
        public Window
    {
    public :

        struct Properties
        {
            std::string title;
            unsigned int width;
            unsigned int height;
            glm::vec4 background_color;
            bool v_sync;
            Window* self;
        };

        DesktopWindow(const WindowProperties& props);
        DesktopWindow(const DesktopWindow&) = default;
        DesktopWindow(DesktopWindow&&) = default;
        DesktopWindow& operator=(const DesktopWindow&) = default;
        DesktopWindow& operator=(DesktopWindow&&) = default;

        virtual ~DesktopWindow();

        void beginRender2d() const override;

        void onUpdate() override;

        unsigned int getWidth() const override { return properties.width; }
        unsigned int getHeight() const override { return properties.height; }

        void clear();
        void setBackgroundColor(const glm::vec4& color);
        const glm::vec4& getBackgroundColor() const;

        void setVSync(bool enabled) override;
        bool isVSync() const override{ return properties.v_sync; }

        void makeCurrent() const override { 
            glfwMakeContextCurrent(window_); }
        void shutdown() override;
    private:
        void init(const WindowProperties& props);
    private :
        Properties properties;
        bool cleared_{};
        bool open{true};

        GLFWwindow* window_;
        GLuint vao_2d_;

    protected:
        glm::vec2 getMousePosition() const override
        {
            glm::dvec2 mouse_pos;
            glfwGetCursorPos(window_, &mouse_pos.x, &mouse_pos.y);
            return mouse_pos;
        }
    };
}


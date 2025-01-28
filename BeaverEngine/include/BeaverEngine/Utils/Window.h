#pragma once
#include "BeaverEngine/Core/std.h"
#include "BeaverEngine/Core/TypeDef.h"
#include <glm/glm.hpp>

namespace bv
{
    class Window;
    struct WindowProperties
    {
        WindowProperties(std::string_view w_name = "", std::string_view w_title = "Beaver Engine", unsigned int w_width = 1280, unsigned int w_height = 720, glm::vec4 w_color = glm::vec4(0, 0, 0, 1), Window* w_shared = nullptr)
            : name(w_name), title(w_title), width(w_width), height(w_height), color(w_color), shared(w_shared)
        {
            
        }
        std::string name;
        std::string title;
        unsigned int height;
        unsigned int width ;
        glm::vec4 color;
        Window* shared;
    };

    class Window 
    {
        friend class InputSystem;
    public :
        virtual ~Window(){}
        
        virtual void onUpdate() = 0;

        virtual unsigned int getWidth() const = 0;
        virtual unsigned int getHeight() const = 0;
        
        virtual void clear() = 0;
        virtual void setBackgroundColor(const glm::vec4& color) = 0;
        virtual const glm::vec4& getBackgroundColor() const = 0;

        virtual void setVSync(bool enabled) = 0;
        virtual bool isVSync() const = 0;
        virtual void makeCurrent() const = 0;
        virtual void shutdown() = 0;
        virtual void beginRender2d() const = 0;

        const std::string& getName() const { return name_; }

        static std::shared_ptr<Window> create(const WindowProperties& properties = WindowProperties());
    protected :
        
        virtual glm::vec2 getMousePosition() const = 0;
        std::string name_;
    };
}


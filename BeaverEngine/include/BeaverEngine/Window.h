#pragma once
#include "BeaverEngine/std.h"

namespace bv
{
    struct WindowProperties
    {
        WindowProperties(std::string_view w_title = "Beaver Engine", unsigned int w_width = 1280, unsigned int w_height = 720)
            : title(w_title), width(w_width), height(w_height)
        {
            
        }
        std::string title;
        unsigned int height;
        unsigned int width ;
    };

    class Window 
    {
    public :
        virtual ~Window(){}
        
        virtual void onUpdate() = 0;

        virtual unsigned int getWidth() const = 0;
        virtual unsigned int getHeight() const = 0;
        
        virtual void setVSync(bool enabled) = 0;
        virtual bool isVSync() const = 0;

        static Window* create(const WindowProperties& properties = WindowProperties());
    };
}


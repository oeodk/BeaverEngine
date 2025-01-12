#pragma once
#include "BeaverEngine/System/System.h"
#include "BeaverEngine/Component/DisplayComponent.h"

namespace bv
{
    class Renderer;

    class DisplaySystem :
        public System
    {
    public:
        DisplaySystem();
        DisplaySystem(const DisplaySystem&) = default;
        DisplaySystem(DisplaySystem&&) = default;
        DisplaySystem& operator=(const DisplaySystem&) = default;
        DisplaySystem& operator=(DisplaySystem&&) = default;

        ~DisplaySystem() = default;

        void setup();

        void iterate(const Timing& dt) override;

        void registerComponent(DisplayComponent* display_component);
        void removeComponent(DisplayComponent* display_component);

        static DisplaySystem& getInstance();
    private:
        Renderer* renderer_{};
        std::unordered_set<DisplayComponent*> display_components_;
    };
}


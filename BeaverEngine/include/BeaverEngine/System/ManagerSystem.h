#pragma once
#include "BeaverEngine/System/System.h"
#include "BeaverEngine/Component/ManagerComponent.h"

namespace bv
{
    class ManagerSystem :
        public System
    {
    public:
        ManagerSystem() = default;
        ManagerSystem(const ManagerSystem&) = default;
        ManagerSystem(ManagerSystem&&) = default;
        ManagerSystem& operator=(const ManagerSystem&) = default;
        ManagerSystem& operator=(ManagerSystem&&) = default;

        ~ManagerSystem() = default;

        void iterate(const Timing& dt) override;

        void registerComponent(ManagerComponent* manager_component);
        void removeComponent(ManagerComponent* manager_component);

        static ManagerSystem& getInstance();
    private:
        std::unordered_set<ManagerComponent*> manager_components_;
    };
}


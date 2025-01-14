#pragma once
#include "BeaverEngine/System/System.h"
#include "BeaverEngine/Component/ColliderComponent.h"

namespace bv
{
    class ColliderSystem :
        public System
    {
    public:
        ColliderSystem() = default;
        ColliderSystem(const ColliderSystem&) = default;
        ColliderSystem(ColliderSystem&&) = default;
        ColliderSystem& operator=(const ColliderSystem&) = default;
        ColliderSystem& operator=(ColliderSystem&&) = default;

        ~ColliderSystem() = default;

        void iterate(const Timing& dt) override;

        void registerComponent(ColliderComponent* collider_component);
        void removeComponent(ColliderComponent* collider_component);

        static ColliderSystem& getInstance();
    private:
        std::unordered_set<ColliderComponent*> collider_components_;
    };
}

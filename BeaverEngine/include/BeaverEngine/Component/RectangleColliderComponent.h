#pragma once
#include "BeaverEngine/Component/ColliderComponent.h"
#include <glm/glm.hpp>

namespace bv
{
    class CircleColliderComponent;

    class RectangleColliderComponent :
        public ColliderComponent
    {
    public:
        friend class CircleColliderComponent;

        static constexpr auto type_ = "RectangleCollider";
        RectangleColliderComponent(Entity& owner) : Component(owner) {}

        bool collides(const ColliderComponent& other) const override { return other.collides(*this); }

        void setup(const ComponentDescription& init_value) override;

        void setRotationAngle(float angle) override;
    private:
        bool collides(const RectangleColliderComponent& other) const override;
        bool collides(const CircleColliderComponent& other) const override;
        
        glm::vec2 half_size_{};
        glm::vec2 true_center_{};
        std::array<glm::vec2, 4> points_{};
        std::array<glm::vec2, 4> rectangles_perpendicular_edge_vector_{};
    };
}


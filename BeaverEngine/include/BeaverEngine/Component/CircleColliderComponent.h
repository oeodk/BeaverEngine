#pragma once
#include "BeaverEngine/Component/ColliderComponent.h"

namespace bv
{
    class RectangleColliderComponent;
	class CircleColliderComponent
		: public ColliderComponent
	{
    public:
        friend class RectangleColliderComponent;

        static constexpr auto type_ = "CircleCollider";
        CircleColliderComponent(Entity& owner) : Component(owner) {}

        bool collides(const ColliderComponent& other) const override { return other.collides(*this); }
       
        void setup(const ComponentDescription& init_value) override;

        void setRotationAngle(float angle) override;

    private:
        bool collides(const RectangleColliderComponent& other) const override;
        bool collides(const CircleColliderComponent& other) const override;

        float radius_{};
        glm::vec2 true_center_{};
	};
}

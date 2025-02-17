#pragma once
#include "BeaverEngine/Component/ColliderComponent.h"
#include "BeaverEngine/Utils/Array2D.h"
namespace bv
{
    class RectangleColliderComponent;
    class CircleColliderComponent;

	class IntGridColliderComponent
		: public ColliderComponent
	{
    public:
        friend class RectangleColliderComponent;
        friend class CircleColliderComponent;

        static constexpr auto type_ = "IntGridCollider";
        IntGridColliderComponent(Entity& owner) : Component(owner) {}
        IntGridColliderComponent(const IntGridColliderComponent&) = default;
        IntGridColliderComponent& operator=(const IntGridColliderComponent&) = default;
        IntGridColliderComponent(IntGridColliderComponent&&) = default;
        IntGridColliderComponent& operator=(IntGridColliderComponent&&) = default;

        bool collides(const ColliderComponent& other) const override { return other.collides(*this); }
       
        void setup(const ComponentDescription& init_value) override;

        void setRotationAngle(float angle) override {}

        std::unordered_map<const ColliderComponent*, glm::vec2>& getColliderTileMap() { return collider_tile_map_; }

    private:
        bool collides(const RectangleColliderComponent& other) const override;
        bool collides(const CircleColliderComponent& other) const override;
        bool collides(const IntGridColliderComponent& other) const override { return false; }

        bool testCollision(const std::array<glm::vec2, 4>& dp, const glm::vec2& other_position, int other_mask, const ColliderComponent* other) const;

        Array2D<int> int_grid_;
        unsigned int grid_size_{1};

        mutable std::unordered_map<const ColliderComponent*, glm::vec2> collider_tile_map_;
	};
}

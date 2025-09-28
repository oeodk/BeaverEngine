#pragma once
#include "BeaverEngine/Component/Component.h"
#include "BeaverEngine/Core/TypeDef.h"
#include "BeaverEngine/Component/CollisionComponent.h"
#include <glm/glm.hpp>
namespace bv
{
    class RectangleColliderComponent;
    class CircleColliderComponent;
    class IntGridColliderComponent;

    class ColliderComponent :
        virtual public Component
    {
    public:
        ColliderComponent();
        ~ColliderComponent();
        ColliderComponent(const ColliderComponent&) = default;
        ColliderComponent& operator=(const ColliderComponent&) = default;
        ColliderComponent(ColliderComponent&&) = default;
        ColliderComponent& operator=(ColliderComponent&&) = default;

        virtual bool collides(const ColliderComponent& other) const = 0;
        virtual bool collidesWithPoint(const glm::vec2& other) const { return false; }
        void onCollision(const ColliderComponent& other) ;

        void resolve() override;

        int getMask() const { return mask_; }
        int getFlag() const { return flag_; }

        virtual void setRotationAngle(float angle) = 0;
        void setOffset(glm::vec2 offset) { setOffset(offset.x, offset.y); }
        void setOffset(float offset_x, float offset_y)
        {
            offset_.y = offset_y;
            offset_.x = offset_x;
            setRotationAngle(rotation_angle_);
        }

        float getRotationAngle() const { return rotation_angle_; }
        const glm::vec2& getOffset() const { return offset_; }

        virtual bool collides(const RectangleColliderComponent& other) const = 0;
        virtual bool collides(const CircleColliderComponent& other) const = 0;
        virtual bool collides(const IntGridColliderComponent& other) const = 0;
    protected:

        int flag_{}, mask_{};
        CollisionComponent* collision_handler_ = nullptr;
        std::string handler_type_{};

        float rotation_angle_{};
        glm::vec2 offset_{};
    };
}

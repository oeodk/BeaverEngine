#pragma once
#include "BeaverEngine/Component/Component.h"
#include <glm/glm.hpp>

namespace bv
{
    class PositionComponent :
        public Component
    {
    private :
        glm::vec3 position_{};
    public :
        static constexpr auto type_ = "Position";

        PositionComponent(Entity& owner) : Component(owner) {}
        PositionComponent(const PositionComponent&) = default;
        PositionComponent& operator=(const PositionComponent&) = default;
        PositionComponent(PositionComponent&&) = default;
        PositionComponent& operator=(PositionComponent&&) = default;

        void setup(const ComponentDescription& init_value) override;
        void resolve() override;

        void setPosition(const glm::vec3& new_pos) { setPosition(new_pos.x, new_pos.y, new_pos.z); }
        void setPosition(const glm::vec2& new_pos) { setPosition(new_pos.x, new_pos.y, position_.z); }
        void setPosition(float x, float y) { setPosition(x, y, position_.z); }
        void setPosition(float x, float y, float z);

        // Set position relative to the parent component
        void setRelativePosition(const glm::vec3& new_pos) { setRelativePosition(new_pos.x, new_pos.y, new_pos.z); }
        // Set position relative to the parent component
        void setRelativePosition(const glm::vec2& new_pos) { setRelativePosition(new_pos.x, new_pos.y, position_.z); }
        // Set position relative to the parent component
        void setRelativePosition(float x, float y) { setRelativePosition(x, y, position_.z); }
        // Set position relative to the parent component
        void setRelativePosition(float x, float y, float z);

        void move(const glm::vec3& new_pos) { move(new_pos.x, new_pos.y, new_pos.z); }
        void move(const glm::vec2& new_pos) { move(new_pos.x, new_pos.y, 0.f); }
        void move(float x, float y) { move(x, y, 0.f); }
        void move(float x, float y, float z);

        const glm::vec3& getWorldPosition() const { return position_; }
        // Get position relative to the parent component
        glm::vec3 getRelativePosition() const;

        //clamp method?
    };
}


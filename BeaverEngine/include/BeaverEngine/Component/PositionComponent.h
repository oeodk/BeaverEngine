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
        void setup(const ComponentDescription& init_value) override;

        void setPosition(const glm::vec3& new_pos) { setPosition(new_pos.x, new_pos.y, new_pos.z); }
        void setPosition(const glm::vec2& new_pos) { setPosition(new_pos.x, new_pos.y, 0); }
        void setPosition(float x, float y, float z)
        {
            position_.x = x;
            position_.y = y;
            position_.z = z;
        }

        // Set position relative to the parent component
        void setRelativePosition(const glm::vec3& new_pos) { setRelativePosition(new_pos.x, new_pos.y, new_pos.z); }
        // Set position relative to the parent component
        void setRelativePosition(const glm::vec2& new_pos) { setRelativePosition(new_pos.x, new_pos.y, 0); }
        // Set position relative to the parent component
        void setRelativePosition(float x, float y, float z);

        const glm::vec3& getWorldPosition() const { return position_; }
        // Get position relative to the parent component
        glm::vec3 getRelativePosition() const;

        //clamp method?
    };
}


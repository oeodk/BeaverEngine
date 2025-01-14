#pragma once
#include "BeaverEngine/Component/Component.h"

namespace bv
{
    class ColliderComponent;

    class CollisionComponent :
        virtual public Component
    {
    public:
        virtual void onCollision(const ColliderComponent& other) = 0;
    };
}


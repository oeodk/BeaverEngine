#pragma once
#include "BeaverEngine/std.h"
#include "BeaverEngine/TypeDef.h"
#include "BeaverEngine/System/System.h"

namespace bv
{
    class EntitySystem :
        public System
    {
    public :
        EntitySystem() = default;
        EntitySystem(const EntitySystem&) = default;
        EntitySystem(EntitySystem&&) = default;
        EntitySystem& operator=(const EntitySystem&) = default;
        EntitySystem& operator=(EntitySystem&&) = default;

        ~EntitySystem() = default;

        void iterate(const Timing& dt) override { entity_to_remove_.clear(); }

        static EntitySystem& getInstance();
        static void remove(const EntityRef& entity);
    private :
        std::vector<EntityRef> entity_to_remove_;
    };
}


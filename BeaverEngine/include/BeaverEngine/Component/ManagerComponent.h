#pragma once
#include "BeaverEngine/Component/Component.h"
#include "BeaverEngine/Utils/Timing.h"

namespace bv
{
    class ManagerComponent :
        public Component
    {
    public :
        ManagerComponent(Entity& owner);
        virtual ~ManagerComponent();
        virtual void update(const Timing& dt) = 0;

        template<typename T>
        static T* getManager(){
            return static_cast<T*>(getManagerInternal(T::type_));
        }
    private:
        static Component* getManagerInternal(std::string_view type);
    };
}


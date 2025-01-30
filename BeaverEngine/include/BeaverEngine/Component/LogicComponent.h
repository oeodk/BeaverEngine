#pragma once
#include "BeaverEngine/Component/Component.h"
#include "BeaverEngine/Utils/Timing.h"

namespace bv
{
    class LogicComponent :
        public virtual Component
    {
    public :
        LogicComponent();
        ~LogicComponent();
        LogicComponent(const LogicComponent&) = default;
        LogicComponent& operator=(const LogicComponent&) = default;
        LogicComponent(LogicComponent&&) = default;
        LogicComponent& operator=(LogicComponent&&) = default;

        virtual void updateLogic(const Timing& timing) = 0;
    };
}


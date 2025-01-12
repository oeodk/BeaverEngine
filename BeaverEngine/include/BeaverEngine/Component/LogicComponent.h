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
        virtual void updateLogic(const Timing& timing) = 0;
    };
}


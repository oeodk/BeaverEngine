#pragma once
#include "BeaverEngine/Component/Component.h"
#include "BeaverEngine/DataStruct/Timing.h"

namespace bv
{
    class LogicComponent :
        public virtual Component
    {
    public :
        inline LogicComponent();
        inline ~LogicComponent();
        virtual void updateLogic(const Timing& timing) = 0;
    };
}


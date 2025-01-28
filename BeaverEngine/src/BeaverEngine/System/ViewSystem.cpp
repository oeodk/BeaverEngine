#include "BeaverEngine/System/ViewSystem.h"

namespace bv
{
    ViewSystem& ViewSystem::getInstance()
    {
        static ViewSystem view_system;
        return view_system;
    }
}

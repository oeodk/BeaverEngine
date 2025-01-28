#include "BeaverEngine/System/TextureSystem.h"

namespace bv
{
    TextureSystem& TextureSystem::getInstance()
    {
        static TextureSystem texture_system;
        return texture_system;
    }
}

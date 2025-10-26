#include "BeaverEngine/Platform/PlatformMacros.h"
#ifdef PLATFORM_WEB
#include <BeaverEngine/Platform/Web/WebControllerRumble.h>

namespace bv
{
	std::unique_ptr<ControllerRumble> ControllerRumble::create()
	{
		return std::make_unique<WebControllerRumble>();
	}

}
#endif
#include <BeaverEngine/Platform/Desktop/DesktopControllerRumble.h>

namespace bv
{
	std::unique_ptr<ControllerRumble> ControllerRumble::create()
	{
		return std::make_unique<DesktopControllerRumble>();
	}

}
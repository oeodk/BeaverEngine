#include "BeaverEngine/Core/std.h"
#include "BeaverEngine/Core/KeyCode.h"

namespace bv
{
	class ControllerRumble
	{
	public:
		virtual void update() = 0;
		virtual bool makeControllerRumble(Joystick::Joystick controller_index, uint16_t low_frequency_rumble, uint16_t high_frequency_rumble, uint32_t duration_ms) = 0;
	
		static std::unique_ptr<ControllerRumble> create();

	};
}
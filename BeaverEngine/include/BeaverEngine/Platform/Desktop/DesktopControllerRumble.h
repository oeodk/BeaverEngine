#include <SDL3/SDL.h>
#include <SDL3/SDL_gamepad.h>
#include "BeaverEngine/Core/KeyCode.h"
#include "BeaverEngine/Utils/ControllerRumble.h"
#include <iostream>
namespace bv
{
	class DesktopControllerRumble
		: public ControllerRumble
	{
	public:
		DesktopControllerRumble()
		{
			SDL_SetHint(SDL_HINT_XINPUT_ENABLED, "1");  // before SDL_Init
			if (SDL_Init(SDL_INIT_GAMEPAD | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_EVENTS) == 0) {
				std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
			}
		}

		~DesktopControllerRumble()
		{
			SDL_Quit();
		}

		void update() override
		{
			SDL_Event e;
			while (SDL_PollEvent(&e)) {}
			SDL_UpdateJoysticks();
		}
		bool makeControllerRumble(Joystick::Joystick controller_index, uint16_t low_frequency_rumble, uint16_t high_frequency_rumble, uint32_t duration_ms) override
		{
			int count = 0;
			SDL_JoystickID* ids = SDL_GetGamepads(&count);
			SDL_Gamepad* gamepad = NULL;
			if (count > controller_index)
			{
				return SDL_RumbleGamepad(SDL_OpenGamepad(ids[controller_index]), low_frequency_rumble, high_frequency_rumble, duration_ms);
			}
			return false;
		}
	private:
	};
}


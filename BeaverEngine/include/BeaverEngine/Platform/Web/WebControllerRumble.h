#pragma once
#include "BeaverEngine/Platform/PlatformMacros.h"
#ifdef PLATFORM_WEB

#include <SDL.h>
#include <SDL_joystick.h>
#include "BeaverEngine/Core/KeyCode.h"
#include "BeaverEngine/Utils/ControllerRumble.h"
#include <iostream>
namespace bv
{
	class WebControllerRumble
		: public ControllerRumble
	{
	public:
		WebControllerRumble()
		{
			SDL_SetHint(SDL_HINT_XINPUT_ENABLED, "1");  // before SDL_Init
			if (SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_EVENTS) == 0) {
				std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
			}
		}

		~WebControllerRumble()
		{
			SDL_Quit();
		}

		void update() override
		{
			SDL_Event e;
			while (SDL_PollEvent(&e)) {}
			//SDL_UpdateJoysticks();
		}

		bool makeControllerRumble(Joystick::Joystick controller_index, uint16_t low_frequency_rumble, uint16_t high_frequency_rumble, uint32_t duration_ms) override
		{
			SDL_Joystick* joystick = SDL_JoystickOpen(static_cast<int>(controller_index));
			if (joystick) {
				SDL_JoystickRumble(joystick, low_frequency_rumble, high_frequency_rumble, duration_ms);
				SDL_JoystickClose(joystick);

			}
			return false;
		}
	private:
	};
}

#endif
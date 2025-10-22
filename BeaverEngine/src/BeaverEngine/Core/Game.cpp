#include "BeaverEngine/Core/Game.h"
#include "BeaverEngine/Platform/PlatformMacros.h"
#include "BeaverEngine/BeaverEngine.h"
#include "BeaverEngine/Core/Debug.h"
#ifdef _WIN32
#include <windows.h>
#endif

namespace bv
{
	using namespace std::chrono_literals;

	PLATEFORM_MAIN_LOOP
	

	bool bv::Game::run()
	{
#ifndef SHIPPING
		static bool path_defined = false;
		if(!path_defined)
		{
			path_defined = true;
			defineDebugDataPath();
		}

#else
#ifdef _WIN32
		HWND hwnd = GetConsoleWindow();
		//Sleep(1);//If you execute these code immediately after the program starts, you must wait here for a short period of time, otherwise GetWindow will fail. I speculate that it may be because the console has not been fully initialized.
		HWND owner = GetWindow(hwnd, GW_OWNER);
		if (owner == NULL) {
			ShowWindow(hwnd, SW_HIDE); // Windows 10
		}
		else {
			ShowWindow(owner, SW_HIDE);// Windows 11
		}
#endif
#endif // !SHIPPING

		initializeUtils();

		registerBasicComponent();
		registerComponent();
		setupSystem();


		addSystem<bv::EntitySystem>();

		Scene::load(Descr::load("./data/scene/default_scene.yaml"));

		for (auto& system : systems_)
		{
			if (auto* display_system = dynamic_cast<DisplaySystem*>(system))
			{
				display_system->setup();
			}
			if (auto*particle_system = dynamic_cast<ParticleSystem*>(system))
			{
				particle_system->setup();
			}
		}

		framerate_ms_ = static_cast<std::chrono::milliseconds>(static_cast<int>((1 / fps_) * 1000));
		auto old_time = std::chrono::high_resolution_clock::now();
		bool& close_application = closeApplication();

		

		unsigned int frame = 0;
		MAIN_LOOP(old_time, frame, close_application, this, 
			loopBody(old_time, frame, close_application);
		)

		for (unsigned int frame = 0; !close_application; frame++)
		{
		}

		return true;
	}

	void Game::initializeUtils()
	{
		Random::init();
	}

	void Game::loopBody(std::chrono::steady_clock::time_point& old_time, unsigned int& frame, bool& close_application)
	{
		auto current_time = std::chrono::high_resolution_clock::now();
		auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - old_time);

		if (dt < framerate_ms_)
		{
			std::this_thread::sleep_for(framerate_ms_ - dt);
			current_time = std::chrono::high_resolution_clock::now();
			dt = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - old_time);
		}
		old_time = current_time;
		for (auto& system : systems_)
		{
			system->iterate({ dt, frame });
		}

#ifndef SHIPPING

		auto current_time2 = std::chrono::high_resolution_clock::now();

		auto dt2 = std::chrono::duration_cast<std::chrono::milliseconds>(current_time2 - old_time);

		float count = dt2.count();
		if (count != 0)
		{
			printf((std::to_string(int(count)) + " ms\n").c_str());
			printf((std::to_string(int(1 / (count / 1000.f))) + " fps\n").c_str());
		}
#endif
	}
}
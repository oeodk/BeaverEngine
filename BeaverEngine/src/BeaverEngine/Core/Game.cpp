#include "BeaverEngine/Core/Game.h"
#include "BeaverEngine/BeaverEngine.h"
#include "BeaverEngine/Core/Debug.h"
namespace bv
{
	using namespace std::chrono_literals;

	bool bv::Game::run()
	{
#ifndef SHIPPING
		defineDebugDataPath();
#endif // !SHIPPING


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
		}

		std::chrono::milliseconds framerate_ms = static_cast<std::chrono::milliseconds>(static_cast<int>((1 / fps_) * 1000));
		auto old_time = std::chrono::high_resolution_clock::now();
		for (unsigned int frame = 0; !closeApplication(); frame++)
		{
			auto current_time = std::chrono::high_resolution_clock::now();
			auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - old_time);

			float count = dt.count();
			if (count != 0)
			{
				//printf((std::to_string(int(count)) + " ms\n").c_str());
				//printf((std::to_string(int(1 / (count / 1000.f)))+ " fps\n").c_str());
			}
			if (dt < framerate_ms)
			{
				std::this_thread::sleep_for(framerate_ms - dt);
				current_time = std::chrono::high_resolution_clock::now();
				dt = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - old_time);
			}
			old_time = current_time;
			for (auto& system : systems_)
			{
				system->iterate({ dt, frame });
			}
		}

		return true;
	}
}
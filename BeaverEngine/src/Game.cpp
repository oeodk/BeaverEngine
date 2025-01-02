#include "BeaverEngine/Game.h"
#include "BeaverEngine/System/EntitySystem.h"
#include "BeaverEngine/Scene.h"
#include "BeaverEngine/Entity.h"
#include "BeaverEngine/Functions/components.h"
namespace bv
{
	using namespace std::chrono_literals;

	Game::Game(const WindowProperties& props)
	{
		window_ = std::unique_ptr<Window>(Window::create(props));
	}

	bool bv::Game::run()
	{
		registerBasicComponent();
		registerComponent();
		setupSystem();

		addSystem<bv::EntitySystem>();

		//Scene::load(init_function_);

		std::chrono::milliseconds framerate_ms = static_cast<std::chrono::milliseconds>(static_cast<int>((1 / fps_) * 1000));

		auto old_time = std::chrono::high_resolution_clock::now();
		for (unsigned int frame = 0; !closeApplication(); frame++)
		{
			auto current_time = std::chrono::high_resolution_clock::now();
			auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - old_time);

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
			window_->onUpdate();
		}

		return true;
	}
}
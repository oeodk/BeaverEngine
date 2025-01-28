#include "BeaverEngine/System/WindowSystem.h"
#include "BeaverEngine/Core/Game.h"
namespace bv
{
	WindowSystem& WindowSystem::getInstance()
	{
		static WindowSystem window_system;
		return window_system;
	}

	void WindowSystem::iterate(const Timing& dt)
	{
		for (auto& window : windows_)
		{
			window.second->onUpdate();
		}
	}
	void WindowSystem::closeWindow(std::string_view window_name)
	{
		std::weak_ptr<Window> window_to_close = getWindow(window_name);
		if (window_to_close.lock().get() == main_window_.lock().get())
		{
			Game::close();
		}
		else
		{
			if (!window_to_close.expired())
			{
				window_to_close.lock()->shutdown();
			}
			windows_.erase(std::string(window_name));
		}
	}
}

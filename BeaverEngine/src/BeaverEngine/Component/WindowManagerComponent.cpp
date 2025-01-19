#include "BeaverEngine/Component/WindowManagerComponent.h"
#include "BeaverEngine/Core/Game.h"
namespace bv
{
	void WindowManagerComponent::update(const Timing& dt)
	{
		for (auto& window : windows_)
		{
			window.second->onUpdate();
		}
	}
	void WindowManagerComponent::closeWindow(std::string_view window_name)
	{
		std::weak_ptr<Window> window_to_close = getWindow(window_name);
		if (window_to_close.lock().get() == main_window_.lock().get())
		{
			Game::close();
		}
		else
		{
			window_to_close.lock()->shutdown();
			windows_.erase(std::string(window_name));
		}
	}
}

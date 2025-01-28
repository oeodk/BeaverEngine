#include "BeaverEngine/System/InputSystem.h"
#include "BeaverEngine/System/WindowSystem.h"
namespace bv
{
	InputSystem& InputSystem::getInstance()
	{
		static InputSystem input_system;
		return input_system;
	}

	bool InputSystem::isKeyPressed(Key::Key key)
	{
		return keys_state_[key].state == State::PRESSED;
	}

	bool InputSystem::isMouseButtonPressed(Mouse::Button button)
	{
		return mouse_buttons_state_[button].state == State::PRESSED;
	}

	bool InputSystem::isKeyReleased(Key::Key key)
	{
		return keys_state_[key].state == State::RELEASED;
	}

	bool InputSystem::isMouseButtonReleased(Mouse::Button button)
	{
		return mouse_buttons_state_[button].state == State::RELEASED;
	}

	bool InputSystem::isKeyHeld(Key::Key key)
	{
		return keys_state_[key].state == State::HELD || keys_state_[key].state == State::PRESSED;
	}

	bool InputSystem::isMouseButtonHeld(Mouse::Button button)
	{
		return mouse_buttons_state_[button].state == State::HELD ||mouse_buttons_state_[button].state == State::PRESSED;
	}

	glm::vec2 InputSystem::getScreenMousePosition()
	{
		return WindowSystem::getInstance().getMainWindow().lock()->getMousePosition();
	}

	glm::vec2 InputSystem::getNormalizedMousePosition()
	{
		auto window = WindowSystem::getInstance().getMainWindow().lock();
		glm::vec2 pos = window->getMousePosition();
		pos.x /= window->getWidth();
		pos.y /= window->getHeight();
		return pos;
	}

	glm::vec2 InputSystem::getScreenMousePosition(std::string_view window_name)
	{
		return WindowSystem::getInstance().getWindow(window_name).lock()->getMousePosition();
	}

	glm::vec2 InputSystem::getNormalizedMousePosition(std::string_view window_name)
	{
		auto window = WindowSystem::getInstance().getWindow(window_name).lock();
		glm::vec2 pos = window->getMousePosition();
		pos.x /= window->getWidth();
		pos.y /= window->getHeight();
		return pos;
	}

	int InputSystem::getMouseScrollDirection()
	{
		return mouse_scroll_value_ / std::abs(mouse_scroll_value_);
	}

	float InputSystem::getMouseScrollValue()
	{
		return mouse_scroll_value_;
	}
	
}
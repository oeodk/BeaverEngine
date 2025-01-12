#include "BeaverEngine/Component/InputManagerComponent.h"
#include "BeaverEngine/Component/WindowManagerComponent.h"
namespace bv
{
	bool InputManagerComponent::isKeyPressed(Key::Key key)
	{
		return keys_state_[key].state == State::PRESSED;
	}

	bool InputManagerComponent::isMouseButtonPressed(Mouse::Button button)
	{
		return mouse_buttons_state_[button].state == State::PRESSED;
	}

	bool InputManagerComponent::isKeyReleased(Key::Key key)
	{
		return keys_state_[key].state == State::RELEASED;
	}

	bool InputManagerComponent::isMouseButtonReleased(Mouse::Button button)
	{
		return mouse_buttons_state_[button].state == State::RELEASED;
	}

	bool InputManagerComponent::isKeyHeld(Key::Key key)
	{
		return keys_state_[key].state == State::HELD || keys_state_[key].state == State::PRESSED;
	}

	bool InputManagerComponent::isMouseButtonHeld(Mouse::Button button)
	{
		return mouse_buttons_state_[button].state == State::HELD ||mouse_buttons_state_[button].state == State::PRESSED;
	}

	glm::vec2 InputManagerComponent::getScreenMousePosition()
	{
		return ManagerComponent::getManager<WindowManagerComponent>()->getMainWindow().lock()->getMousePosition();
	}

	glm::vec2 InputManagerComponent::getNormalizedMousePosition()
	{
		auto window = ManagerComponent::getManager<WindowManagerComponent>()->getMainWindow().lock();
		glm::vec2 pos = window->getMousePosition();
		pos.x /= window->getWidth();
		pos.y /= window->getHeight();
		return pos;
	}

	glm::vec2 InputManagerComponent::getScreenMousePosition(std::string_view window_name)
	{
		return ManagerComponent::getManager<WindowManagerComponent>()->getWindow(window_name).lock()->getMousePosition();
	}

	glm::vec2 InputManagerComponent::getNormalizedMousePosition(std::string_view window_name)
	{
		auto window = ManagerComponent::getManager<WindowManagerComponent>()->getWindow(window_name).lock();
		glm::vec2 pos = window->getMousePosition();
		pos.x /= window->getWidth();
		pos.y /= window->getHeight();
		return pos;
	}

	int InputManagerComponent::getMouseScrollDirection()
	{
		return mouse_scroll_value_ / std::abs(mouse_scroll_value_);
	}

	float InputManagerComponent::getMouseScrollValue()
	{
		return mouse_scroll_value_;
	}
	
}
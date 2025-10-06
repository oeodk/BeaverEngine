#include "BeaverEngine/System/InputSystem.h"
#include "BeaverEngine/System/WindowSystem.h"
#include <BeaverEngine/Utils/ControllerRumble.h>
namespace bv
{
	InputSystem::InputSystem()
	{
		controller_rumble_ = ControllerRumble::create();
	}

	InputSystem& InputSystem::getInstance()
	{
		static InputSystem input_system;
		return input_system;
	}

	void InputSystem::iterate(const Timing& dt)
	{
		for (auto& key : updated_keys_)
		{
			switch (keys_state_.at(key).state)
			{
			case State::PRESSED:
				keys_state_.at(key).state = State::HELD;
				break;
			case State::RELEASED:
				keys_state_.at(key).state = State::NONE;
				break;
			}
		}
		updated_keys_.clear();

		for (auto& button : updated_mouse_buttons_)
		{
			switch (mouse_buttons_state_.at(button).state)
			{
			case State::PRESSED:
				mouse_buttons_state_.at(button).state = State::HELD;
				break;
			case State::RELEASED:
				mouse_buttons_state_.at(button).state = State::NONE;
				break;
			}
		}
		updated_mouse_buttons_.clear();

		if (updated_controllers_buttons_.size() != 0)
		{
			for (auto& joystick_buttons : updated_controllers_buttons_)
			{
				for (auto& button : joystick_buttons.second)
				{
					switch (controllers_buttons_state_.at(joystick_buttons.first).at(button).state)
					{
					case State::PRESSED:
						controllers_buttons_state_.at(joystick_buttons.first).at(button).state = State::HELD;
						break;
					case State::RELEASED:
						controllers_buttons_state_.at(joystick_buttons.first).at(button).state = State::NONE;
						break;
					}
				}
			}
		}
		updated_controllers_buttons_.clear();

		mouse_scroll_value_ = 0;

		controller_rumble_->update();
	}

	bool InputSystem::isKeyPressed(Key::Key key)
	{
		return keys_state_[key].state == State::PRESSED;
	}

	bool InputSystem::isMouseButtonPressed(Mouse::Button button)
	{
		return mouse_buttons_state_[button].state == State::PRESSED;
	}

	bool InputSystem::isControllerButtonPressed(Gamepad::Gamepad button, Joystick::Joystick joystick)
	{
		return controllers_buttons_state_[joystick][button].state == State::PRESSED;
	}

	bool InputSystem::isKeyReleased(Key::Key key)
	{
		return keys_state_[key].state == State::RELEASED;
	}

	bool InputSystem::isMouseButtonReleased(Mouse::Button button)
	{
		return mouse_buttons_state_[button].state == State::RELEASED;
	}

	bool InputSystem::isControllerButtonReleased(Gamepad::Gamepad button, Joystick::Joystick joystick)
	{
		return controllers_buttons_state_[joystick][button].state == State::RELEASED;
	}

	bool InputSystem::isKeyHeld(Key::Key key)
	{
		return keys_state_[key].state == State::HELD || keys_state_[key].state == State::PRESSED;
	}

	bool InputSystem::isMouseButtonHeld(Mouse::Button button)
	{
		return mouse_buttons_state_[button].state == State::HELD ||mouse_buttons_state_[button].state == State::PRESSED;
	}

	bool InputSystem::isControllerButtonHeld(Gamepad::Gamepad button, Joystick::Joystick joystick)
	{
		return controllers_buttons_state_[joystick][button].state == State::HELD || controllers_buttons_state_[joystick][button].state == State::PRESSED;
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
		if (mouse_scroll_value_ == 0)
		{
			return 0;
		}
		return mouse_scroll_value_ / std::abs(mouse_scroll_value_);
	}

	float InputSystem::getMouseScrollValue()
	{
		return mouse_scroll_value_;
	}

	int InputSystem::getJoystickAxisDirection(Axis::Axis axis, Joystick::Joystick joystick)
	{
		float value = joystick_axis_value[joystick][axis];
		if (value == 0)
		{
			return value;
		}
		return value / std::abs(value);
	}

	float InputSystem::getJoystickAxisValue(Axis::Axis axis, Joystick::Joystick joystick)
	{
		return joystick_axis_value[joystick][axis];
	}
	
	bool InputSystem::makeControllerRumble(Joystick::Joystick controller_index, uint16_t low_frequency_rumble, uint16_t high_frequency_rumble, uint32_t duration_ms)
	{
		if (controllers_buttons_state_.size() > controller_index)
		{
			return controller_rumble_->makeControllerRumble(controller_index, low_frequency_rumble, high_frequency_rumble, duration_ms);
		}
		return false;
	}
}
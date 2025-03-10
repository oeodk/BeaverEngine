#pragma once
#include "BeaverEngine/System/System.h"

#include "BeaverEngine/Core/KeyCode.h"
#include "BeaverEngine/Core/std.h"
#include <glm/glm.hpp>
namespace bv
{
	class Window;
	class InputSystem
		: public System
	{
	public:
		friend class Window;
		struct State
		{
			enum Value
			{
				RELEASED = 0, PRESSED = 1, HELD, NONE
			};
			Value state;

			State(Value value = NONE) : state(value){}
		};

		inline static float dead_zone_ = 0.1;

		InputSystem() = default;
		InputSystem(const InputSystem&) = default;
		InputSystem(InputSystem&&) = default;
		InputSystem& operator=(const InputSystem&) = default;
		InputSystem& operator=(InputSystem&&) = default;

		~InputSystem() = default;

		static InputSystem& getInstance();

		void iterate(const Timing& dt) override
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

			if(updated_controllers_buttons_.size() != 0)
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
		}

		bool isKeyPressed(Key::Key key);
		bool isMouseButtonPressed(Mouse::Button button);
		bool isControllerButtonPressed(Gamepad::Gamepad button, Joystick::Joystick joystick = Joystick::JOYSTICK_1);
		
		bool isKeyReleased(Key::Key key);
		bool isMouseButtonReleased(Mouse::Button button);
		bool isControllerButtonReleased(Gamepad::Gamepad button, Joystick::Joystick joystick = Joystick::JOYSTICK_1);

		bool isKeyHeld(Key::Key key);
		bool isMouseButtonHeld(Mouse::Button button);
		bool isControllerButtonHeld(Gamepad::Gamepad button, Joystick::Joystick joystick = Joystick::JOYSTICK_1);

		// In main window coords
		glm::vec2 getScreenMousePosition();
		glm::vec2 getNormalizedMousePosition();

		glm::vec2 getScreenMousePosition(std::string_view window_name);
		glm::vec2 getNormalizedMousePosition(std::string_view window_name);

		// -1 scroll down, 1 scroll up
		int getMouseScrollDirection();
		// < 0 scroll down, > 0 scroll up
		float getMouseScrollValue();

		int getJoystickAxisDirection(Axis::Axis axis, Joystick::Joystick joystick = Joystick::JOYSTICK_1);
		float getJoystickAxisValue(Axis::Axis axis, Joystick::Joystick joystick = Joystick::JOYSTICK_1);


		void setKeyState(Key::Key key, bool value)
		{ 
			keys_state_[key].state = State::Value(value); 
			updated_keys_.push_back(key);
		}

		void setMouseButtonState(Mouse::Button button, bool value)
		{
			mouse_buttons_state_[button] = State::Value(value);
			updated_mouse_buttons_.push_back(button);
		}
		
		
		void setControllerButtonState(Joystick::Joystick joystick, Gamepad::Gamepad button, bool value)
		{
			if(!value && (controllers_buttons_state_[joystick][button].state == State::PRESSED || controllers_buttons_state_[joystick][button].state == State::HELD)
		    || value && (controllers_buttons_state_[joystick][button].state == State::NONE || controllers_buttons_state_[joystick][button].state == State::RELEASED))
			{
				controllers_buttons_state_[joystick][button] = State::Value(value);
				updated_controllers_buttons_[joystick].push_back(button);
			}
		}

		void setMouseScrollValue(float value)
		{
			mouse_scroll_value_ = value;
		}
		
		void setJoystickAxisValue(Joystick::Joystick joystick, Axis::Axis axis ,float value)
		{
			if (abs(value) < dead_zone_)
			{
				joystick_axis_value[joystick][axis] = 0;
			}
			else
			{
				joystick_axis_value[joystick][axis] = value;
			}
		}

	private:
		std::unordered_map<Key::Key, State> keys_state_{};
		std::unordered_map<Mouse::Button, State> mouse_buttons_state_{};
		std::map<Joystick::Joystick, std::unordered_map<Gamepad::Gamepad, State>> controllers_buttons_state_{};
		std::map<Joystick::Joystick, std::unordered_map<Axis::Axis, float>> joystick_axis_value{};

		std::vector<Key::Key> updated_keys_{};
		std::vector<Mouse::Button> updated_mouse_buttons_{};
		std::unordered_map<Joystick::Joystick, std::vector<Gamepad::Gamepad>> updated_controllers_buttons_{};

		float mouse_scroll_value_{};
	};
}


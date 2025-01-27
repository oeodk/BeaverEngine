#pragma once
#include "BeaverEngine/Component/ManagerComponent.h"

#include "BeaverEngine/Core/KeyCode.h"
#include "BeaverEngine/Core/std.h"
#include <glm/glm.hpp>
namespace bv
{
	class InputManagerComponent
		: public ManagerComponent
	{
	public:
		struct State
		{
			enum Value
			{
				RELEASED = 0, PRESSED = 1, HELD, NONE
			};
			Value state;

			State(Value value = NONE) : state(value){}
		};

		static constexpr auto type_ = "InputManager";
		InputManagerComponent(Entity& owner) : ManagerComponent(owner) {}
		void setup(const ComponentDescription& init_value) override {}
		void update(const Timing& dt) override
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
			mouse_scroll_value_ = 0;
		}

		bool isKeyPressed(Key::Key key);
		bool isMouseButtonPressed(Mouse::Button button);
		
		bool isKeyReleased(Key::Key key);
		bool isMouseButtonReleased(Mouse::Button button);

		bool isKeyHeld(Key::Key key);
		bool isMouseButtonHeld(Mouse::Button button);

		// In main window coords
		glm::vec2 getScreenMousePosition();
		glm::vec2 getNormalizedMousePosition();

		glm::vec2 getScreenMousePosition(std::string_view window_name);
		glm::vec2 getNormalizedMousePosition(std::string_view window_name);

		// -1 scroll down, 1 scroll up
		int getMouseScrollDirection();
		// < 0 scroll down, > 0 scroll up
		float getMouseScrollValue();

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

		void setMouseScrollValue(float value)
		{
			mouse_scroll_value_ = value;
		}
	private:
		std::unordered_map<Key::Key, State> keys_state_{};
		std::unordered_map<Mouse::Button, State> mouse_buttons_state_{};

		std::vector<Key::Key> updated_keys_{};
		std::vector<Mouse::Button> updated_mouse_buttons_{};

		float mouse_scroll_value_{};
	};
}


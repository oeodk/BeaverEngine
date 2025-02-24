#include "Paddle.h"
#include <BeaverEngine/System/InputSystem.h>
#include <BeaverEngine/Core/Entity.h>
#include <BeaverEngine/Component/PositionComponent.h>
#include <BeaverEngine/Component/ColliderComponent.h>
#include <BeaverEngine/Tools/LdtkLevelComponent.h>
#include <BeaverEngine/System/AudioSystem.h>
#include <BeaverEngine/System/WindowSystem.h>
#include <BeaverEngine/Core/GlobalConstants.h>
#include <BeaverEngine/Component/WindowComponent.h>
#include "Ball.h"
#include "WindowPong.h"

void example::Paddle::setup(const bv::ComponentDescription& init_value)
{
	win_.setup(bv::constants::SOUNDS_PATH + "win.wav");
	is_left_ = init_value.parameters.at("isLeft").as<bool>();
	if (is_left_)
	{
		up_ = bv::Key::W;
		down_ = bv::Key::S;
		controller_ = bv::Joystick::JOYSTICK_1;
	}
	else
	{
		up_ = bv::Key::UP;
		down_ = bv::Key::DOWN;
		controller_ = bv::Joystick::JOYSTICK_2;
	}
}

void example::Paddle::resolve()
{
	auto window = owner().getComponent<bv::WindowComponent>()->getWindow();
	screen_size_ratio_ = static_cast<glm::vec2>(bv::WindowSystem::getInstance().getMonitorSize()) / WindowPong::REF_SCREEN_SIZE;
	glm::vec2 window_base_size {window->getWidth(), window->getHeight()};
	window_base_size *= screen_size_ratio_;
	if (is_left_)
	{
		window->setPosition((-(WindowPong::REF_SCREEN_SIZE.x / 2.f) * 0.95) * screen_size_ratio_.x, 0);
		owner().getComponent<bv::PositionComponent>()->setPosition(-(WindowPong::REF_SCREEN_SIZE.x / 2.f) * 0.95, 0);
	}
	else
	{
		window->setPosition(((WindowPong::REF_SCREEN_SIZE.x / 2.f) * 0.95) * screen_size_ratio_.x, 0);
		owner().getComponent<bv::PositionComponent>()->setPosition((WindowPong::REF_SCREEN_SIZE.x / 2.f) * 0.95, 0);
	}
}

void example::Paddle::updateLogic(const bv::Timing& timing)
{
	auto window = owner().getComponent<bv::WindowComponent>()->getWindow();
	
	if (bv::InputSystem::getInstance().isKeyHeld(up_) || bv::InputSystem::getInstance().getJoystickAxisDirection(bv::Axis::LEFT_Y, controller_) > 0)
	{
		if(owner().getComponent<bv::PositionComponent>()->getWorldPosition().y + paddle_size.y / 2.f < (WindowPong::REF_SCREEN_SIZE.y / 2.f))
		{
			owner().getComponent<bv::PositionComponent>()->move(0, speed_, 0);
			window->setPosition(glm::vec2(owner().getComponent<bv::PositionComponent>()->getWorldPosition()) * screen_size_ratio_);
		}
	
	}
	if (bv::InputSystem::getInstance().isKeyHeld(down_) || bv::InputSystem::getInstance().getJoystickAxisDirection(bv::Axis::LEFT_Y, controller_) < 0)
	{
		if(owner().getComponent<bv::PositionComponent>()->getWorldPosition().y  - paddle_size.y / 2.f > -(WindowPong::REF_SCREEN_SIZE.y / 2.f))
		{
			owner().getComponent<bv::PositionComponent>()->move(0, -speed_, 0);
			window->setPosition(glm::vec2(owner().getComponent<bv::PositionComponent>()->getWorldPosition()) * screen_size_ratio_);
		}
	}

	if (bv::InputSystem::getInstance().isKeyHeld(bv::Key::ESCAPE))
	{
		bv::Game::close();
	}
}

void example::Paddle::onCollision(const bv::ColliderComponent& other)
{
}

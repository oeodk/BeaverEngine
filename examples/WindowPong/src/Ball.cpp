#include "Ball.h"
#include <BeaverEngine/Core/Entity.h>
#include <BeaverEngine/Component/PositionComponent.h>
#include <BeaverEngine/Component/ColliderComponent.h>
#include <BeaverEngine/Tools/LdtkLevelComponent.h>
#include <BeaverEngine/Functions/Math.h>
#include <BeaverEngine/System/InputSystem.h>
#include <BeaverEngine/System/AudioSystem.h>
#include <BeaverEngine/System/WindowSystem.h>
#include <BeaverEngine/Core/GlobalConstants.h>
#include "Paddle.h"
#include "WindowPong.h"

void example::Ball::setup(const bv::ComponentDescription& init_value)
{
	music_.setup(bv::constants::MUSICS_PATH + "music.wav");
	music_.setGain(0.5);
	hit_.setup(bv::constants::SOUNDS_PATH + "hit.wav");
	death_.setup(bv::constants::SOUNDS_PATH + "lose.wav");

	bv::AudioSystem::getInstance().playMusic(music_, this);
	if (rand() % 2)
	{
		direction_.x = 1;
	}
	else
	{
		direction_.x = -1;
	}
}

void example::Ball::resolve()
{
	auto window = owner().getComponent<bv::WindowComponent>()->getWindow();

	screen_size_ratio_ = static_cast<glm::vec2>(bv::WindowSystem::getInstance().getMonitorSize()) / WindowPong::REF_SCREEN_SIZE;
}

void example::Ball::updateLogic(const bv::Timing& timing)
{
	auto window = owner().getComponent<bv::WindowComponent>()->getWindow();
	owner().getComponent<bv::PositionComponent>()->move(speed_ * direction_);
	window->setPosition(glm::vec2(owner().getComponent<bv::PositionComponent>()->getWorldPosition()) * screen_size_ratio_);

	float ball_y = owner().getComponent<bv::PositionComponent>()->getWorldPosition().y;
	if (ball_y + ball_radius_ < -(WindowPong::REF_SCREEN_SIZE.y / 2.f)
		|| ball_y + ball_radius_ >(WindowPong::REF_SCREEN_SIZE.y / 2.f))
	{
		direction_.y *= -1;
		bv::AudioSystem::getInstance().playSound(hit_, this);
	}
	
	float ball_x = owner().getComponent<bv::PositionComponent>()->getWorldPosition().x;
	if (ball_x - ball_radius_ < -(WindowPong::REF_SCREEN_SIZE.x / 2.f)
		|| ball_x + ball_radius_ > (WindowPong::REF_SCREEN_SIZE.x / 2.f))
	{
		owner().getComponent<bv::PositionComponent>()->setPosition(0, 0);
		window->setPosition(0, 0);
		direction_.y = 0;
		if (rand() % 2)
		{
			direction_.x = 1;
		}
		else
		{
			direction_.x = -1;
		}
		bv::AudioSystem::getInstance().playSound(death_, this);
	}
}

void example::Ball::onCollision(const bv::ColliderComponent& other)
{
	bv::AudioSystem::getInstance().playSound(hit_, this);

	if (const auto paddle = other.owner().getComponent<Paddle>())
	{
		if (bv::InputSystem::getInstance().isKeyHeld(paddle->up_) || bv::InputSystem::getInstance().getJoystickAxisDirection(bv::Axis::LEFT_Y, paddle->controller_) > 0)
		{
			if(direction_.y < 0.55)
				direction_.y += 0.3;
		}
		if (bv::InputSystem::getInstance().isKeyHeld(paddle->down_) || bv::InputSystem::getInstance().getJoystickAxisDirection(bv::Axis::LEFT_Y, paddle->controller_) < 0)
		{
			if (direction_.y > -0.55)
			direction_.y -= 0.3;
		}
		direction_.x *= -1;
		direction_ = glm::normalize(direction_);
		bv::AudioSystem::getInstance().playSound(hit_, this);
	}
}

#include "Ball.h"
#include <BeaverEngine/Core/Entity.h>
#include <BeaverEngine/Component/PositionComponent.h>
#include <BeaverEngine/Component/ColliderComponent.h>
#include <BeaverEngine/Tools/LdtkLevelComponent.h>
#include <BeaverEngine/Functions/Math.h>
#include <BeaverEngine/System/InputSystem.h>
#include <BeaverEngine/System/AudioSystem.h>
#include <BeaverEngine/Core/GlobalConstants.h>
#include "Paddle.h"
#include "Brick.h"

void example::Ball::setup(const bv::ComponentDescription& init_value)
{
	music_.setup(bv::constants::MUSICS_PATH + "music.wav");
	music_.setGain(0.5);
	hit_.setup(bv::constants::SOUNDS_PATH + "hit.wav");
	death_.setup(bv::constants::SOUNDS_PATH + "lose.wav");

	bv::AudioSystem::getInstance().playMusic(music_, this);
}

void example::Ball::updateLogic(const bv::Timing& timing)
{
	owner().getComponent<bv::PositionComponent>()->move(speed_ * direction_);
	if (owner().getComponent<bv::PositionComponent>()->getWorldPosition().y < -165)
	{
		if(speed_ != 0)
		{
			speed_ = 0;
			bv::AudioSystem::getInstance().playSound(death_, this);
		}
	}
}

void example::Ball::onCollision(const bv::ColliderComponent& other)
{
	bv::AudioSystem::getInstance().playSound(hit_, this);

	if (other.owner().getComponent<Paddle>())
	{
		if (bv::InputSystem::getInstance().isKeyHeld(bv::Key::RIGHT))
		{
			if(direction_.x< 0.55)
				direction_.x += 0.3;
		}
		if (bv::InputSystem::getInstance().isKeyHeld(bv::Key::LEFT))
		{
			if (direction_.x > -0.55)
			direction_.x -= 0.3;
		}
		direction_.y *= -1;
		direction_ = glm::normalize(direction_);
		bv::AudioSystem::getInstance().playSound(hit_, this);
	}
	if (other.owner().getComponent<bv::LdtkLevelComponent>())
	{
		auto& pos = owner().getComponent<bv::PositionComponent>()->getWorldPosition();
		if (pos.y > 140)
		{
			direction_.y *= -1;
		}
		else
		{
			direction_.x *= -1;
		}
	}
	if (auto brick = other.owner().getComponent<Brick>())
	{
		const glm::vec2 brick_pos  = other.owner().getComponent<bv::PositionComponent>()->getWorldPosition();
		const glm::vec2 brick_corner_pos1  = brick_pos + brick->getSize() / 2.f;
		const glm::vec2 brick_corner_pos2  = brick_corner_pos1 + brick->getSize().x;

		const float a1 = (brick_pos.y - brick_corner_pos1.y) / (brick_pos.x - brick_corner_pos1.x);
		const float b1 = (brick_pos.y - a1 * brick_pos.x);
		
		const float a2 = (brick_pos.y - brick_corner_pos2.y) / (brick_pos.x - brick_corner_pos2.x);
		const float b2 = (brick_pos.y - a2 * brick_pos.x);

		const glm::vec3& ball_pos = owner().getComponent<bv::PositionComponent>()->getWorldPosition();

		if ((ball_pos.y < a1 * ball_pos.x + b1 && ball_pos.y < a2 * ball_pos.x + b2) || (ball_pos.y > a1 * ball_pos.x + b1 && ball_pos.y > a2 * ball_pos.x + b2))
		{
			direction_.y *= -1;
		}
		else
		{
			direction_.x *= -1;
		}
	}
}

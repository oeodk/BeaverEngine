#include "Paddle.h"
#include <BeaverEngine/System/InputSystem.h>
#include <BeaverEngine/Core/Entity.h>
#include <BeaverEngine/Component/PositionComponent.h>
#include <BeaverEngine/Component/ColliderComponent.h>
#include <BeaverEngine/Tools/LdtkLevelComponent.h>
#include <BeaverEngine/System/AudioSystem.h>
#include <BeaverEngine/Core/GlobalConstants.h>
#include "Brick.h"
#include "Ball.h"

void example::Paddle::setup(const bv::ComponentDescription& init_value)
{
	win_.setup(bv::constants::SOUNDS_PATH + "win.wav");
}

void example::Paddle::updateLogic(const bv::Timing& timing)
{
	if (Brick::brick_count == 0)
	{
		if (Ball::speed_ != 0)
		{
			Ball::speed_ = 0;
			bv::AudioSystem::getInstance().playSound(win_, this);
		}
	}
	else
	{
		if (bv::InputSystem::getInstance().isKeyHeld(bv::Key::RIGHT))
		{
			owner().getComponent<bv::PositionComponent>()->move(speed_, 0, 0);
		}
		if (bv::InputSystem::getInstance().isKeyHeld(bv::Key::LEFT))
		{
			owner().getComponent<bv::PositionComponent>()->move(-speed_, 0, 0);
		}
	}
}

void example::Paddle::onCollision(const bv::ColliderComponent& other)
{
}

#include "Brick.h"
#include <BeaverEngine/Core/Entity.h>
#include <BeaverEngine/Component/SpriteComponent.h>
#include <BeaverEngine/Component/RectangleColliderComponent.h>
#include <BeaverEngine/System/EntitySystem.h>
#include <BeaverEngine/System/AudioSystem.h>
#include <BeaverEngine/Core/GlobalConstants.h>

void example::Brick::setup(const bv::ComponentDescription& init_value)
{
	life_ = init_value.parameters.at("life").as<int>();
	brick_break_.setup(bv::constants::SOUNDS_PATH + "break.wav");
}

void example::Brick::resolve()
{
	owner().getComponent<bv::SpriteComponent>()->setColor(life_color_[life_]);
}

void example::Brick::updateLogic(const bv::Timing& dt)
{
	if (disappear)
	{
		timer += dt.dt_.count();
		if (timer > 200)
		{
			owner().getParent()->removeChild(owner());
		}
	}
}

void example::Brick::onCollision(const bv::ColliderComponent& other)
{
	if (life_ > 0)
	{
		life_--;
		owner().getComponent<bv::SpriteComponent>()->setColor(life_color_[life_]);
	}
	else
	{
		bv::AudioSystem::getInstance().playSound(brick_break_, this);
		owner().getComponent<bv::SpriteComponent>()->setAnimationName("break");
		owner().getComponent<bv::SpriteComponent>()->resetAnimationDt();
		owner().getComponent<bv::RectangleColliderComponent>()->disable();
		disappear = true;
	}
}

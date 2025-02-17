#include "TestComponent.h"
#include "BeaverEngine/System/InputSystem.h"
#include <BeaverEngine/Core/Entity.h>
#include <BeaverEngine/Component/PositionComponent.h>
#include <BeaverEngine/Component/SpriteComponent.h>

namespace sandbox
{
	void TestComponent::setup(const bv::ComponentDescription& init_value)
	{
		test_var_ = init_value.parameters.at("testVar").as<int>();
	}

	std::string getSpriteName(int value)
	{
		switch (value)
		{
		case -4:
			return "bottom_left_walk";
			break;
		case -3:
			return "bottom_walk";
			break;
		case -2:
			return "bottom_right_walk";
			break;
		case -1:
			return "left_walk";
			break;
		case 0:
			return "idle";
			break;
		case 1:
			return "right_walk";
			break;
		case 2:
			return "top_left_walk";
			break;
		case 3:
			return "top_walk";
			break;
		case 4:
			return "top_right_walk";
			break;

		}
		return "";
	}

	void TestComponent::updateLogic(const bv::Timing& timing)
	{
		auto& input = bv::InputSystem::getInstance();
		float x = 0, y = 0;
		float speed = 0.5;
		if (input.isKeyHeld(bv::Key::W))
		{
			y += speed;
		}
		if (input.isKeyHeld(bv::Key::S))
		{
			y -= speed;
		}
		if (input.isKeyHeld(bv::Key::D))
		{
			x += speed;
		}
		if (input.isKeyHeld(bv::Key::A))
		{
			x -= speed;
		}

		int v = x + y * 3;
		auto* sprite = owner().getComponent<bv::SpriteComponent>();
		sprite->setAnimationName(getSpriteName(v));

		auto position = owner().getComponent<bv::PositionComponent>();
		position->setPosition(position->getWorldPosition() + glm::vec3(x, y,0) * 10.f);

		if (collide_)
		{
			sprite->setColor({ 1,0,0,1 });
			collide_ = false;
		}
		else
		{
			//sprite->setColor({ 1,1,1,1 });

		}
	}
	void TestComponent::onCollision(const bv::ColliderComponent& other)
	{
		collide_ = true;
	}
}
#include "BrickBreaker.h"

#include "Brick.h"
#include "Paddle.h"
#include "Ball.h"

namespace example
{
	void BrickBreaker::setupSystem()
	{
		addSystem<bv::WindowSystem>();
		addSystem<bv::ViewSystem>();
		addSystem<bv::TextureSystem>();
		addSystem<bv::AudioSystem>();
		addSystem<bv::LogicSystem>();
		addSystem<bv::ColliderSystem>();
		addSystem<bv::DisplaySystem>();
		addSystem<bv::InputSystem>();
	}
	void BrickBreaker::registerComponent() const
	{
		bv::Component::Register<Ball>();
		bv::Component::Register<Brick>();
		bv::Component::Register<Paddle>();
	}
}



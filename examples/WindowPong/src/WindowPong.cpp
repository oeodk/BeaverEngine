#include "WindowPong.h"

#include "Paddle.h"
#include "Ball.h"

namespace example
{
	void WindowPong::setupSystem()
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
	void WindowPong::registerComponent() const
	{
		bv::Component::Register<Ball>();
		bv::Component::Register<Paddle>();
	}
}



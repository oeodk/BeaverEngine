#include "Game.h"
#include "TestComponent.h"
#include "TestColliderComponent.h"
#include "TestEntitySpawnerComponent.h"
namespace sandbox
{
	void Game::setupSystem()
	{
		addSystem<bv::WindowSystem>();
		addSystem<bv::ViewSystem>();
		addSystem<bv::TextureSystem>();
		addSystem<bv::AudioSystem>();
		addSystem<bv::ColliderSystem>();
		addSystem<bv::LogicSystem>();
		addSystem<bv::DisplaySystem>();
		addSystem<bv::InputSystem>();
	}
	void Game::registerComponent() const
	{
		bv::Component::Register<TestComponent>();
		bv::Component::Register<TestColliderComponent>();
		bv::Component::Register<TestEntitySpawnerComponent>();
	}
}



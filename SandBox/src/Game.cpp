#include "Game.h"
#include "TestComponent.h"
#include "TestColliderComponent.h"
#include "TestEntitySpawnerComponent.h"
namespace sandbox
{
	void Game::setupSystem()
	{
		addSystem<bv::ColliderSystem>();
		addSystem<bv::LogicSystem>();
		addSystem<bv::DisplaySystem>();
	}
	void Game::registerComponent() const
	{
		bv::Component::Register<TestComponent>();
		bv::Component::Register<TestColliderComponent>();
		bv::Component::Register<TestEntitySpawnerComponent>();
	}
}



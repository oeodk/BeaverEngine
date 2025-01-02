#include "Game.h"
#include <BeaverEngine/System/EntitySystem.h>
#include <BeaverEngine/System/LogicSystem.h>
namespace sandbox
{

	Game::Game(const bv::WindowProperties& props)
		: bv::Game(props)
	{
	}

	void Game::setupSystem()
	{
		addSystem<bv::LogicSystem>();
	}
	void Game::registerComponent() const
	{

	}
}



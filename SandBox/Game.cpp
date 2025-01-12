#include "Game.h"

namespace sandbox
{
	void Game::setupSystem()
	{
		addSystem<bv::LogicSystem>();
		addSystem<bv::DisplaySystem>();
	}
	void Game::registerComponent() const
	{

	}
}



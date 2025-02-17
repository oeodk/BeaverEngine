#pragma once
#include <BeaverEngine/Core/Game.h>

namespace sandbox
{
	class Game : public bv::Game
	{
	public:
		Game() = default;
		void setupSystem() override;
		void registerComponent() const override;

#ifndef SHIPPING
		void defineDebugDataPath() override
		{
			std::filesystem::path new_path = "../../SandBox/";
			std::filesystem::current_path(new_path);
		}
#endif // !SHIPPING
	};
}
#pragma once
#include <BeaverEngine/Core/Game.h>

namespace example
{
	class BrickBreaker : public bv::Game
	{
	public:
		BrickBreaker() = default;
		void setupSystem() override;
		void registerComponent() const override;



#ifndef SHIPPING
		void defineDebugDataPath() override
		{
			std::filesystem::path new_path = "../../../examples/BrickBreaker/";
			std::filesystem::current_path(new_path);
		}
#endif // !SHIPPING
	};
}
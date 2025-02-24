#pragma once
#include <BeaverEngine/Core/Game.h>

namespace example
{
	class WindowPong : public bv::Game
	{
	public:
		WindowPong() = default;
		void setupSystem() override;
		void registerComponent() const override;

		inline static constexpr glm::vec2 REF_SCREEN_SIZE{1920, 1080};

#ifndef SHIPPING
		void defineDebugDataPath() override
		{
			std::filesystem::path new_path = "../../../examples/WindowPong/";
			std::filesystem::current_path(new_path);
		}
#endif // !SHIPPING
	};
}
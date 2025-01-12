#include <BeaverEngine/Core/Game.h>

namespace sandbox
{
	class Game : public bv::Game
	{
	public:
		Game() = default;
		void setupSystem() override;
		void registerComponent() const override;
	};
}
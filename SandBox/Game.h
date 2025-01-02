#include <BeaverEngine/Game.h>

namespace sandbox
{
	class Game : public bv::Game
	{
	public:
		Game(const bv::WindowProperties& props);
		void setupSystem() override;
		void registerComponent() const override;
	};
}
#include <BeaverEngine/Component/Component.h>
#include <BeaverEngine/Component/LogicComponent.h>
#include <BeaverEngine/Utils/Sound.h>
#include <BeaverEngine/Utils/SoundPlayer.h>

namespace sandbox
{
	class TestEntitySpawnerComponent
		: public bv::LogicComponent
	{
	public:
		static constexpr auto type_ = "TestEntitySpawnerComponent";
		TestEntitySpawnerComponent(bv::Entity& owner) : Component(owner) {}
		void setup(const bv::ComponentDescription& init_value) override;
		void resolve() override;
		void updateLogic(const bv::Timing& dt) override;
	private:
		bv::Description component_template_;
		bv::Sound sound_;
		bv::SoundPlayer* player_;
	};
}
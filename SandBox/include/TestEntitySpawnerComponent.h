#include <BeaverEngine/Component/Component.h>
#include <BeaverEngine/Component/LogicComponent.h>
#include <BeaverEngine/Utils/AudioData.h>
#include <BeaverEngine/Utils/AudioPlayer.h>

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
		bv::AudioData sound_;
		bv::AudioData music_;
		bv::AudioPlayer* player_;
	};
}
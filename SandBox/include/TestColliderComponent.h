#pragma once
#include <BeaverEngine/Component/LogicComponent.h>
#include <BeaverEngine/Component/CollisionComponent.h>

namespace bv
{
	class ColliderComponent;
}
namespace sandbox
{
	class TestColliderComponent :
		public bv::LogicComponent, public bv::CollisionComponent
	{
	public:
		static constexpr auto type_ = "ColliderTestComponent";

		TestColliderComponent(bv::Entity& owner) : Component(owner) {}
		void setup(const bv::ComponentDescription& init_value) override {}

		void updateLogic(const bv::Timing& timing) override;
		void onCollision(const bv::ColliderComponent& other) override;

	private:
		bool collide_ = false;
	};
}



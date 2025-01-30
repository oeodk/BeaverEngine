#pragma once
#include <BeaverEngine/Component/LogicComponent.h>
#include <BeaverEngine/Component/CollisionComponent.h>


namespace sandbox
{
	class bv::ColliderComponent;

	class TestComponent :
		public bv::LogicComponent, public bv::CollisionComponent
	{
	public:
		static constexpr auto type_ = "TestComponent";

		TestComponent(bv::Entity& owner) : Component(owner) {}
		void setup(const bv::ComponentDescription& init_value) override;

		void updateLogic(const bv::Timing& timing) override;
		void onCollision(const bv::ColliderComponent& other) override;

	private:
		bool collide_ = false;
		int test_var_{};
	};
}


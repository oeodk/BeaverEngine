#pragma once
#include <BeaverEngine/Component/LogicComponent.h>
#include <BeaverEngine/Component/CollisionComponent.h>
#include <BeaverEngine/Utils/AudioData.h>


namespace example
{
	class bv::ColliderComponent;

	class Paddle :
		public bv::LogicComponent, public bv::CollisionComponent
	{
	public:
		static constexpr auto type_ = "Paddle";

		Paddle(bv::Entity& owner) : Component(owner) {}
		void setup(const bv::ComponentDescription& init_value) override;

		void updateLogic(const bv::Timing& timing) override;
		void onCollision(const bv::ColliderComponent& other) override;

	private:
		float speed_ = 5;
		bv::AudioData win_;

	};
}


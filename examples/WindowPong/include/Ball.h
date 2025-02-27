#pragma once
#include <BeaverEngine/Component/LogicComponent.h>
#include <BeaverEngine/Component/CollisionComponent.h>
#include <glm/glm.hpp>
#include <BeaverEngine/Utils/AudioData.h>

namespace example
{
	class bv::ColliderComponent;

	class Ball :
		public bv::LogicComponent, public bv::CollisionComponent
	{
	public:
		static constexpr auto type_ = "Ball";

		Ball(bv::Entity& owner) : Component(owner) {}
		void setup(const bv::ComponentDescription& init_value) override;
		void resolve() override;

		void updateLogic(const bv::Timing& timing) override;
		void onCollision(const bv::ColliderComponent& other) override;

		inline static float speed_ = 15;
	private:
		glm::vec2 direction_{0,0};
		float ball_radius_ = 10;
		
		bv::AudioData music_;
		bv::AudioData hit_;
		bv::AudioData death_;

		glm::vec2 screen_size_ratio_;
	};
}


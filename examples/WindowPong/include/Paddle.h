#pragma once
#include <BeaverEngine/Component/LogicComponent.h>
#include <BeaverEngine/Component/CollisionComponent.h>
#include <BeaverEngine/Utils/AudioData.h>
#include <BeaverEngine/Core/KeyCode.h>

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
		void resolve() override;

		void updateLogic(const bv::Timing& timing) override;
		void onCollision(const bv::ColliderComponent& other) override;

		bv::Key::Key up_, down_;
		bv::Joystick::Joystick controller_;
	private:
		float speed_ = 10;
		bv::AudioData win_;
		bool is_left_ = false;

		inline static const glm::vec2 paddle_size{30, 250};

		inline static glm::vec2 screen_size_ratio_{};
	};
}


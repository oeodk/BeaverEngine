#pragma once
#include <BeaverEngine/Component/CollisionComponent.h>
#include <BeaverEngine/Component/LogicComponent.h>
#include <glm/glm.hpp>
#include <BeaverEngine/Utils/AudioData.h>

namespace example
{
	class bv::ColliderComponent;

	class Brick :
		public bv::CollisionComponent, public bv::LogicComponent
	{
	public:
		static constexpr auto type_ = "Brick";

		Brick(bv::Entity& owner) : Component(owner) { brick_count++; }
		~Brick() { brick_count--; }
		void setup(const bv::ComponentDescription& init_value) override;

		void resolve() override;

		void updateLogic(const bv::Timing& dt) override;
		void onCollision(const bv::ColliderComponent& other) override;

		const glm::vec2& getSize() const { return size_; }

		inline static int brick_count = 0;

	private:
		glm::vec2 size_{ 50,11 };
		bool disappear = false;
		float timer = 0;
		int life_;
		std::array<glm::vec3, 3> life_color_ = { glm::vec3(0,1,0), glm::vec3(1,1,0), glm::vec3(1,0,0) };
		bv::AudioData brick_break_;

	};
}


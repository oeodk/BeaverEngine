#pragma once
#include "BeaverEngine/Component/Component.h"

namespace bv
{
	class ParticleComponent
		: public Component
	{
	public:
		static constexpr auto type_ = "Particle";

		ParticleComponent(Entity& owner) : Component(owner) {}
		~ParticleComponent();
		ParticleComponent(const ParticleComponent&) = default;
		ParticleComponent& operator=(const ParticleComponent&) = default;
		ParticleComponent(ParticleComponent&&) = default;
		ParticleComponent& operator=(ParticleComponent&&) = default;

		void setup(const ComponentDescription& init_value) override;
		void resolve() override;

	private:
		float speed_;
		float lifespan_;

	};
}



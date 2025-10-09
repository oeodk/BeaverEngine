#pragma once
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

#include "BeaverEngine/Utils/Timing.h"

#include "BeaverEngine/Utils/Vertex.h"
namespace bv
{
	struct ParticleProps
	{
		glm::vec3 position;
		float rotation_angle = 0.f;
		float lifespan_s = 1.f, lifespan_variation;
		glm::vec2 start_size, end_size;
		glm::vec4 start_color, end_color;
		glm::vec3 velocity, velocity_variation;

	};

	struct Particle
	{
	public:
		void emit(const ParticleProps& props);
		void update(const Timing& dt);
	public:
		glm::vec3 position;
		float rotation_angle = 0.f;

		float lifespan = 1.f, lifespan_remaining = 0.f;
		glm::vec2 start_size, end_size;
		glm::vec4 start_color, end_color;


		glm::vec2 size;
		glm::vec4 color;

		glm::vec3 velocity;

		bool active = false;

		std::array<Vertex2D, 4> vertices;
		std::array<glm::vec2, 4> texture_coords;

	};
}
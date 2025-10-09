#include "BeaverENgine/Utils/Particle.h"
#include "BeaverEngine/Utils/Math.h"
namespace bv
{

	void Particle::emit(const ParticleProps& props)
	{
		position = props.position;
		rotation_angle = props.rotation_angle;

		lifespan = props.lifespan_s + props.lifespan_variation * (Random::randomFloat() - 0.5) * 2;
		lifespan_remaining = lifespan;

		start_size = props.start_size;
		end_size = props.end_size;
		size = start_size;

		start_color = props.start_color;
		end_color = props.end_color;
		color = start_color;

		velocity = props.velocity;
		velocity.x += props.velocity_variation.x * (Random::randomFloat() - 0.5) * 2;
		velocity.y += props.velocity_variation.y * (Random::randomFloat() - 0.5) * 2;
		velocity.z += props.velocity_variation.z * (Random::randomFloat() - 0.5) * 2;

		active = true;

		texture_coords[0] = {0.f, 0.f};
		texture_coords[1] = {1.f, 0.f};
		texture_coords[2] = {1.f, 1.f};
		texture_coords[3] = {0.f, 1.f};
	}

	void Particle::update(const Timing& dt)
	{
		// handle movement and animations, etc...

		// Particle data update
		lifespan_remaining -= dt.asSecond();
		if (lifespan_remaining <= 0.f)
		{
			active = false;
			return;
		}

		position += velocity * dt.asSecond();

		float life = lifespan_remaining / lifespan;

		color = glm::lerp(end_color, start_color, life);
		size = glm::lerp(end_size, start_size, life);

		// vert and texture update
		
		std::array<glm::vec3, 4> points{};
		points[0].x = -(size.x / 2.f);
		points[0].y = -(size.y / 2.f) ;
		points[0].y = -(size.y / 2.f) ;

		points[1].x = -(size.x / 2.f) ;
		points[1].y =  (size.y / 2.f)  ;

		points[2].x =  (size.x / 2.f)  ;
		points[2].y =  (size.y / 2.f)  ;

		points[3].x =  (size.x / 2.f)  ;
		points[3].y = -(size.y / 2.f) ;



		if (rotation_angle != 0.f)
		{
			glm::vec3 axis(0.0f, 0.0f, 1.0f);
			float angle = glm::radians(rotation_angle);

			glm::quat rotation = glm::angleAxis(angle, glm::normalize(axis));
			for (auto& point : points)
			{
				point = rotation * point;
			}
		}

		vertices[0] = { position + points[0], texture_coords[0] , color};
		vertices[1] = { position + points[1], texture_coords[1] , color};
		vertices[2] = { position + points[2], texture_coords[2] , color};
		vertices[3] = { position + points[3], texture_coords[3] , color};
	}
}

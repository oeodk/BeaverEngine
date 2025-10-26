#include "BeaverEngine/Utils/Particle.h"
#include "BeaverEngine/Utils/Math.h"
namespace bv
{

	void bv::Particle::emit(const ParticleProps& props, std::shared_ptr<Texture2D> texture)
	{
		position = props.position;
		rotation_angle = glm::radians(props.rotation_angle);

		// Convert once outside the loop (maybe even outside this function)
		s = glm::sin(rotation_angle * 0.5f);
		c = glm::cos(rotation_angle * 0.5f);

		// Quaternion around Z axis (no normalize needed)
		rotation_ = glm::quat{ c, 0.0f, 0.0f, s };

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
		if(props.animation_name == "")
		{
			texture_coords[0] = { 0.f, 0.f };
			texture_coords[1] = { 1.f, 0.f };
			texture_coords[2] = { 1.f, 1.f };
			texture_coords[3] = { 0.f, 1.f };
		}
		else
		{
			float dt = 0;
			FloatRect texture_rect{ texture->getSpriteCoordinate(props.animation_name, dt), texture->getSpriteSize(props.animation_name) };
		
			if (inv_texture_width == 0 && inv_texture_height == 0)
			{
				inv_texture_width = 1.f / texture->getWidth();
				inv_texture_height = 1.f / texture->getHeight();
			}
			
			texture_coords[0].x = texture_rect.pos.x * inv_texture_width;
			texture_coords[1].y = 1 - texture_rect.pos.y * inv_texture_height;

			texture_coords[1].x = texture_rect.pos.x * inv_texture_width;
			texture_coords[0].y = 1 - (texture_rect.pos.y + texture_rect.size.y) * inv_texture_height;

			texture_coords[2].x = (texture_rect.pos.x + texture_rect.size.x) * inv_texture_width;
			texture_coords[3].y = 1 - (texture_rect.pos.y + texture_rect.size.y) * inv_texture_height;

			texture_coords[3].x = (texture_rect.pos.x + texture_rect.size.x) * inv_texture_width;
			texture_coords[2].y = 1 - texture_rect.pos.y * inv_texture_height;
		}

		vertices[0].texture_coords = texture_coords[0];
		vertices[1].texture_coords = texture_coords[1];
		vertices[2].texture_coords = texture_coords[2];
		vertices[3].texture_coords = texture_coords[3];
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

		color = end_color + (start_color - end_color) * life;
		size = end_size + (start_size - end_size) * life;

		// vert and texture update

		std::array<glm::vec3, 4> points{};
		points[0].x = -(size.x / 2.f);
		points[0].y = -(size.y / 2.f);
		points[0].y = -(size.y / 2.f);

		points[1].x = -(size.x / 2.f);
		points[1].y = (size.y / 2.f);

		points[2].x = (size.x / 2.f);
		points[2].y = (size.y / 2.f);

		points[3].x = (size.x / 2.f);
		points[3].y = -(size.y / 2.f);



		if (rotation_angle != 0.f)
		{
			for (auto& point : points)
			{
				for (auto& p : points)
				{
					float x = p.x * c - p.y * s;
					float y = p.x * s + p.y * c;
					p.x = x;
					p.y = y;
				}
				//point = rotation_ * point;
			}
		}

		vertices[0].position = position + points[0]; 
		vertices[0].color = color;
		vertices[1].position = position + points[1];
		vertices[1].color = color;
		vertices[2].position = position + points[2];
		vertices[2].color = color;
		vertices[3].position = position + points[3];
		vertices[3].color = color;
	}
}

#include "BeaverEngine/Component/ParticleEmitterComponent.h"
#include "BeaverEngine/Component/PositionComponent.h"
#include "BeaverEngine/System/ParticleSystem.h"
#include "BeaverEngine/Core/Entity.h"
#include "BEaverEngine/Utils/Math.h"
namespace bv
{
	void ParticleEmitterComponent::setup(const ComponentDescription& init_value)
	{
		bool end_color_init = false, end_size_init = false;
		
		for (auto& value : init_value.parameters)
		{
			if (string_to_init_enum_map_.contains(value.first))
			{
				switch (string_to_init_enum_map_.at(value.first))
				{
				case bv::ParticleEmitterComponent::START_BEGIN:
					particle_properties_.start_color.r = value.second[0].as<float>();
					particle_properties_.start_color.g = value.second[1].as<float>();
					particle_properties_.start_color.b = value.second[2].as<float>();
					particle_properties_.start_color.a = value.second[3].as<float>();
					break;

				case bv::ParticleEmitterComponent::END_COLOR:
					particle_properties_.end_color.r = value.second[0].as<float>();
					particle_properties_.end_color.g = value.second[1].as<float>();
					particle_properties_.end_color.b = value.second[2].as<float>();
					particle_properties_.end_color.a = value.second[3].as<float>();
					end_color_init = true;
					break;

				case bv::ParticleEmitterComponent::START_SIZE:
					particle_properties_.start_size.x = value.second[0].as<float>();
					particle_properties_.start_size.y = value.second[1].as<float>();
					break;

				case bv::ParticleEmitterComponent::END_SIZE:
					particle_properties_.end_size.x = value.second[0].as<float>();
					particle_properties_.end_size.y = value.second[1].as<float>();
					end_size_init = true;
					break;

				case bv::ParticleEmitterComponent::ANIMATION:
					particle_properties_.animation_name = value.second.as<std::string>();
					break;

				case bv::ParticleEmitterComponent::LIFESPAN:
					particle_properties_.lifespan_s = value.second.as<float>();
					break;

				case bv::ParticleEmitterComponent::LIFESPAN_VARIATION:
					particle_properties_.lifespan_variation = value.second.as<float>();
					break;

				case bv::ParticleEmitterComponent::PRIMITIVE:
					primitive_ = string_to_primitive_map_.at(value.second.as<std::string>());
					break;

				case bv::ParticleEmitterComponent::VELOCITY:
					particle_velocity_ = value.second.as<float>();
					break;

				case bv::ParticleEmitterComponent::VELOCITY_VARIATION:
					particle_velocity_variation_ = value.second.as<float>();

					break;
				case bv::ParticleEmitterComponent::PRIMITIVE_PARAMETER:
					primitive_parameter_ = value.second.as<float>();
					break;

				case bv::ParticleEmitterComponent::EMIT_OUTER:
					emit_outer_ = value.second.as<bool>();
					break;
				
				case bv::ParticleEmitterComponent::EMIT_RADIAL:
					emit_radial = value.second.as<bool>();
					break;
				}
			}
		}
		if (!end_color_init)
		{
			particle_properties_.end_color = particle_properties_.start_color;
		}
		if (!end_size_init)
		{
			particle_properties_.end_size = particle_properties_.end_size;
		}
	}

	void ParticleEmitterComponent::emit(unsigned int particle_count, const glm::vec3& direction)
	{
		
		switch (primitive_)
		{
		case bv::ParticleEmitterComponent::POINT:
			EmitPoint(particle_count, direction);
			break;
		case bv::ParticleEmitterComponent::CIRCLE:
			emitCircle(particle_count, direction);
			break;
		case bv::ParticleEmitterComponent::SQUARE:
			emitSquare(particle_count, direction);
			break;
		case bv::ParticleEmitterComponent::FOUNTAIN:
			emitFountain(particle_count, direction);
			break;
		}
	}

	void ParticleEmitterComponent::emitFountain(unsigned int particle_count, const glm::vec3& direction)
	{
		particle_properties_.position = owner().getComponent<PositionComponent>()->getWorldPosition();
		for (int i = 0; i < particle_count; i++)
		{

			const float angle_variation = Random::randomNormal() * primitive_parameter_;

			float rad = glm::radians(angle_variation);
			float c = glm::cos(rad);
			float s = glm::sin(rad);
			glm::vec3 dir(c * direction.x - s * direction.y, s * direction.x + c * direction.y, direction.z);

			particle_properties_.velocity = dir * particle_velocity_;
			particle_properties_.velocity_variation = dir * particle_velocity_variation_;

			particle_properties_.rotation_angle = Random::randomFloat() * 360;
		ParticleSystem::getInstance().emitParticle(particle_properties_);
		}
	}

	void ParticleEmitterComponent::emitSquare(unsigned int particle_count, const glm::vec3& direction)
	{
		const glm::vec3& position = owner().getComponent<PositionComponent>()->getWorldPosition();
		for (int i = 0; i < particle_count; i++)
		{
			glm::vec2 spawn_position{ bv::Random::randomFloat() - 0.5, bv::Random::randomFloat() - 0.5 };
			spawn_position *= Random::randomFloat() * primitive_parameter_ * 2;
			if (emit_outer_)
			{
				float* v = nullptr;
				if (Random::randomFloat() - 0.5 > 0)
				{
					v = &spawn_position.x;
				}
				else
				{
					v = &spawn_position.y;
				}
				if (*v > 0)
				{
					*v = primitive_parameter_;
				}
				else
				{
					*v = -primitive_parameter_;
				}
			}
			
			particle_properties_.position.x = position.x + spawn_position.x;
			particle_properties_.position.y = position.y + spawn_position.y;
			particle_properties_.position.z = position.z;
			if (direction == NULL_VEC)
			{
				if (emit_radial)
				{
					const glm::vec3 dir{ glm::normalize(spawn_position), 0 };
					particle_properties_.velocity = dir * particle_velocity_;
					particle_properties_.velocity_variation = dir * particle_velocity_variation_;
				}
				else
				{
					const glm::vec3 dir{ bv::Random::randomFloat() - 0.5, bv::Random::randomFloat() - 0.5 ,0 };
					particle_properties_.velocity = dir * particle_velocity_;
					particle_properties_.velocity_variation = dir * particle_velocity_variation_;
				}
				
			}
			else
			{
				particle_properties_.velocity = direction * particle_velocity_;
				particle_properties_.velocity_variation = direction * particle_velocity_variation_;
			}
			particle_properties_.rotation_angle = Random::randomFloat() * 360;
		ParticleSystem::getInstance().emitParticle(particle_properties_);
		}
	}

	void ParticleEmitterComponent::emitCircle(unsigned int particle_count, const glm::vec3& direction)
	{
		const glm::vec3& position = owner().getComponent<PositionComponent>()->getWorldPosition();
		for (int i = 0; i < particle_count; i++)
		{
			glm::vec2 spawn_position = glm::normalize(glm::vec2{ bv::Random::randomFloat() - 0.5, bv::Random::randomFloat() - 0.5 });
			
			if (direction == NULL_VEC)
			{
				if (emit_radial)
				{
					const glm::vec3 dir{ spawn_position , 0 };
					particle_properties_.velocity = dir * particle_velocity_;
					particle_properties_.velocity_variation = dir * particle_velocity_variation_;
				}
				else
				{
					const glm::vec3 dir{ bv::Random::randomFloat() - 0.5, bv::Random::randomFloat() - 0.5 ,0 };
					particle_properties_.velocity = dir * particle_velocity_;
					particle_properties_.velocity_variation = dir * particle_velocity_variation_;
				}
			}
			else
			{
				particle_properties_.velocity = direction * particle_velocity_;
				particle_properties_.velocity_variation = direction * particle_velocity_variation_;
			}

			if (emit_outer_)
			{
				spawn_position *= primitive_parameter_;
			}
			else
			{
				spawn_position *= Random::randomFloat() * primitive_parameter_;
			}
			particle_properties_.position.x = position.x + spawn_position.x;
			particle_properties_.position.y = position.y + spawn_position.y;
			particle_properties_.position.z = position.z;

			particle_properties_.rotation_angle = Random::randomFloat() * 360;
		ParticleSystem::getInstance().emitParticle(particle_properties_);
		}
	}

	void ParticleEmitterComponent::EmitPoint(unsigned int particle_count, const glm::vec3& direction)
	{
		particle_properties_.position = owner().getComponent<PositionComponent>()->getWorldPosition();
		for (int i = 0; i < particle_count; i++)
		{
			if (direction == NULL_VEC)
			{
				const glm::vec3 dir = glm::normalize(glm::vec3{ bv::Random::randomFloat() - 0.5, bv::Random::randomFloat() - 0.5 ,0 });
				particle_properties_.velocity = dir * particle_velocity_;
				particle_properties_.velocity_variation = dir * particle_velocity_variation_;
			}
			else
			{
				particle_properties_.velocity = direction * particle_velocity_;
				particle_properties_.velocity_variation = direction * particle_velocity_variation_;
			}
			particle_properties_.rotation_angle = Random::randomFloat() * 360;
			ParticleSystem::getInstance().emitParticle(particle_properties_);
		}

	}
}


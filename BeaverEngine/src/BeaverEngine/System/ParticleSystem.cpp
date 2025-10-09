#include "BeaverEngine/System/ParticleSystem.h"
#include "BeaverEngine/Core/Entity.h"
#include "BeaverEngine/Core/Renderer.h"
#include "BeaverEngine/Core/std.h"
#include "BeaverEngine/Core/GlobalConstants.h"

#include <BeaverENgine/System/WindowSystem.h>
#include <BeaverENgine/System/ViewSystem.h>
#include <BeaverENgine/System/TextureSystem.h>

namespace bv
{
	ParticleSystem& ParticleSystem::getInstance()
	{
		static ParticleSystem particle_system;
		return particle_system;
	}

	ParticleSystem::ParticleSystem(int particle_pool_size)
		: renderer_(Renderer::getInstance())
	{
		particles_.resize(particle_pool_size);
		current_pool_index = particle_pool_size - 1;
	}

	void ParticleSystem::setup(std::string_view texture_path, bool interpolate)
	{
		renderer_->setup();
		texture_ = TextureSystem::getInstance().getTexture2D(constants::SPRITES_PATH + std::string(texture_path), interpolate);
	}

	void ParticleSystem::emitParticle(const ParticleProps& props)
	{
		particles_[current_pool_index].emit(props);
		current_pool_index = --current_pool_index % particles_.size();
	}


	void ParticleSystem::iterate(const Timing& dt)
	{
		updateParticles(dt);
		renderParticles();
	}

	void ParticleSystem::updateParticles(const Timing& dt)
	{
		for (auto& particle : particles_)
		{
			if (particle.active)
			{
				particle.update(dt);
			}
		}
	}

	void ParticleSystem::renderParticles()
	{
		//batching

		const size_t particle_count = particles_.size();

		auto mapped_vertices = vertex_buffer_.mapVertices(0, 4 * particle_count);
		auto mapped_indices = index_buffer_.mapIndices(0, 6 * particle_count);

		int last_index = 0;
		for (auto& particle : particles_)
		{

			if (particle.active)
			{
				const auto& src_verts = particle.vertices;
				std::transform(src_verts.begin(), src_verts.end(),
					mapped_vertices.begin() + last_index * 4,
					[last_index, particle_count](const Vertex2D& v) {
						Vertex2D vert(v);
						vert.position.z += last_index / static_cast<float>(particle_count);
						return vert;
					});

				const auto& src_indices = INDICES;
				std::transform(src_indices.begin(), src_indices.end(),
					mapped_indices.begin() + last_index * 6,
					[last_index](unsigned int index) { return index + last_index * 4; });

				last_index++;
			}
		}
		if(last_index > 0)
		{
			renderer_->begin2DRender(WindowSystem::getInstance().getMainWindow().lock().get());

			vertex_buffer_.setup();
			index_buffer_.setup();

			texture_->bind();
			renderer_->render(vertex_buffer_, index_buffer_, WindowSystem::getInstance().getMainWindow().lock().get(), ViewSystem::getInstance().getMainView(), last_index * 6);
		
			renderer_->end2DRender();

		}
	}
}

#include "BeaverEngine/System/ParticleSystem.h"

#include "BeaverEngine/Platform/PlatformMacros.h"

#include "BeaverEngine/Core/Entity.h"
#include "BeaverEngine/Core/Renderer.h"
#include "BeaverEngine/Core/std.h"
#include "BeaverEngine/Core/GlobalConstants.h"



namespace bv
{
	ParticleSystem& ParticleSystem::getInstance()
	{
		static ParticleSystem particle_system;
		return particle_system;
	}

	ParticleSystem::ParticleSystem(int particle_pool_size)
		: renderer_(Renderer::getInstance()), pool_size_(particle_pool_size)
	{

	}

	void ParticleSystem::setup(std::string_view texture_path, bool interpolate)
	{
		renderer_->setup();
		texture_ = TextureSystem::getInstance().getTexture2D(constants::SPRITES_PATH + std::string(texture_path), interpolate);
	}

	void ParticleSystem::emitParticle(const ParticleProps& props)
	{
		const auto pair = particles_.begin();
		emitParticle(props, pair->first, particles_.at(pair->first).begin()->first);
	}

	void ParticleSystem::emitParticle(const ParticleProps& props, Window* window, View2D* view)
	{
		uint32_t& current_pool_index_ref = current_pool_index.at(window).at(view);
		particles_.at(window).at(view).at(current_pool_index.at(window).at(view)).emit(props, texture_);
		current_pool_index_ref = --current_pool_index_ref % static_cast<size_t>(pool_size_);
		if (current_pool_index_ref == 0)
		{
			current_pool_index_ref = pool_size_ - 1;
		}
	}

	void ParticleSystem::initializeView(Window* window, View2D* view)
	{
		current_pool_index[window][view] = pool_size_ - 1;
		particles_[window][view].resize(pool_size_);

		vertex_buffer_[window][view] = VertexBuffer<Vertex2D>();
		index_buffer_[window][view] = IndexBuffer();
	}


	void ParticleSystem::iterate(const Timing& dt)
	{
		if (current_pool_index.empty())
		{
			initializeView(WindowSystem::getInstance().getMainWindow().lock().get(), ViewSystem::getInstance().getMainView());
		}
		for (auto& window : particles_)
		{
			for (auto& view : window.second)
			{
				int last_index = 0;

				for (auto& particle : view.second)
				{
					if (particle.active)
					{
						particle.active_index = last_index;
						last_index++;
					}
				}

				auto& particles = view.second;

				const size_t particle_count = particles_.at(window.first).at(view.first).size();

				auto mapped_vertices = vertex_buffer_.at(window.first).at(view.first).mapVertices(0, 4 * particle_count);
				auto mapped_indices = index_buffer_.at(window.first).at(view.first).mapIndices(0, 6 * particle_count);


				

#ifdef PARRALLEL_EXECUTION
				std::for_each(std::execution::par,
					view.second.begin(), view.second.end(),
					[dt, &mapped_vertices, &mapped_indices, particle_count](Particle& particle)
					{
						iterateInternal(particle, dt, mapped_vertices, mapped_indices, particle_count);
					});
#else
				for (auto& particle : view.second)
				{
					iterateInternal(particle, dt, mapped_vertices, mapped_indices, particle_count);
				}
#endif
					
				if (last_index > 0)
				{
					renderer_->begin2DRender(window.first);

					vertex_buffer_.at(window.first).at(view.first).setup();
					index_buffer_.at(window.first).at(view.first).setup();

					texture_->bind();
					renderer_->render(vertex_buffer_.at(window.first).at(view.first), index_buffer_.at(window.first).at(view.first), window.first, view.first, last_index * 6);

					renderer_->end2DRender();

				}
			}
		}
		//updateParticles(dt);
		//renderParticles();
	}

	void ParticleSystem::updateParticles(const Timing& dt)
	{
		for (auto& window : particles_)
		{
			for (auto& view : window.second)
			{

#ifdef PARRALLEL_EXECUTION
				std::for_each(std::execution::par,
					view.second.begin(), view.second.end(),
					[dt](auto& particle)
					{
						if (particle.active)
							particle.update(dt);
					});
#else
				for (auto& particle : view.second)
				{
					if (particle.active)
						particle.update(dt);
				}
#endif // DEBUG

				
			}
		}
		
	}

	void ParticleSystem::renderParticles()
	{
		//batching

		for (auto& window : particles_)
		{
			for (auto& view : window.second)
			{
				auto& particles = view.second;

				const size_t particle_count = particles_.at(window.first).at(view.first).size();

				auto mapped_vertices = vertex_buffer_.at(window.first).at(view.first).mapVertices(0, 4 * particle_count);
				auto mapped_indices = index_buffer_.at(window.first).at(view.first).mapIndices(0, 6 * particle_count);

				int last_index = 0;
				for (auto& particle : particles)
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
					renderer_->begin2DRender(window.first);

					vertex_buffer_.at(window.first).at(view.first).setup();
					index_buffer_.at(window.first).at(view.first).setup();

					texture_->bind();
					renderer_->render(vertex_buffer_.at(window.first).at(view.first), index_buffer_.at(window.first).at(view.first), window.first, view.first, last_index * 6);
		
					renderer_->end2DRender();

				}
			}
		}
	}
	void ParticleSystem::iterateInternal(Particle& particle, const Timing& dt, std::span<Vertex2D>& mapped_vertices, std::span<unsigned int>& mapped_indices, size_t particle_count)
	{
		if (particle.active)
		{
			particle.update(dt);

			const auto& src_verts = particle.vertices;
			std::transform(src_verts.begin(), src_verts.end(),
				mapped_vertices.begin() + particle.active_index * 4,
				[&particle, particle_count](const Vertex2D& v) {
					Vertex2D vert(v);
					vert.position.z += particle.active_index / static_cast<float>(particle_count);
					return vert;
				});

			const auto& src_indices = INDICES;
			std::transform(src_indices.begin(), src_indices.end(),
				mapped_indices.begin() + particle.active_index * 6,
				[&particle](unsigned int index) { return index + particle.active_index * 4; });
		}
	}
}

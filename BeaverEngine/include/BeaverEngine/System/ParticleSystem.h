#pragma once
#include "BeaverEngine/System/System.h"
#include "BeaverEngine/Utils/Particle.h"

#include "BeaverEngine/Utils/Buffer.h"
#include "BeaverEngine/Utils/Texture2D.h"
#include "BeaverEngine/Utils/Vertex.h"

#include <BeaverENgine/System/WindowSystem.h>
#include <BeaverENgine/System/ViewSystem.h>
#include <BeaverENgine/System/TextureSystem.h>

namespace bv
{
    class Renderer;
    class ParticleSystem :
        public System
    {
        public:
            ParticleSystem(int particle_pool_size = 2000);
            ParticleSystem(const ParticleSystem&) = default;
            ParticleSystem(ParticleSystem&&) = default;
            ParticleSystem& operator=(const ParticleSystem&) = default;
            ParticleSystem& operator=(ParticleSystem&&) = default;

            ~ParticleSystem() = default;

            void iterate(const Timing& dt) override;
            void setup(std::string_view texture_path = "particles.png", bool interpolate = false);

            static ParticleSystem& getInstance();

            void emitParticle(const ParticleProps& props);
            void emitParticle(const ParticleProps& props, Window* window, View2D* view);

            void initializeView(Window* window, View2D* view);
            void setPoolSize(int particle_pool_size)
            {
                pool_size_ = particle_pool_size;
            }
        private:
            void updateParticles(const Timing& dt);
            void renderParticles();
        
            std::unordered_map<Window*, std::unordered_map<View2D*, std::vector<Particle>>> particles_;
            std::unordered_map<Window*, std::unordered_map<View2D*, uint32_t >>  current_pool_index;

            Renderer* renderer_{};

            std::shared_ptr<Texture2D> texture_;

            std::unordered_map<Window*, std::unordered_map<View2D*, VertexBuffer<Vertex2D>>> vertex_buffer_;
            std::unordered_map<Window*, std::unordered_map<View2D*, IndexBuffer>> index_buffer_;

            static constexpr std::array<unsigned int, 6> INDICES = { 0,1,2,0,2,3 };
            int pool_size_{};
    };
}


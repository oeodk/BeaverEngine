#pragma once
#include "BeaverEngine/System/System.h"
#include "BeaverEngine/Utils/Particle.h"

#include "BeaverEngine/Utils/Buffer.h"
#include "BeaverEngine/Utils/Texture2D.h"
#include "BeaverEngine/Utils/Vertex.h"

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
        private:
            void updateParticles(const Timing& dt);
            void renderParticles();
        
            std::vector<Particle> particles_;
            int  current_pool_index = 999;

            Renderer* renderer_{};

            std::shared_ptr<Texture2D> texture_;

            VertexBuffer<Vertex2D> vertex_buffer_;
            IndexBuffer index_buffer_;

            static constexpr std::array<unsigned int, 6> INDICES = { 0,1,2,0,2,3 };
    };
}


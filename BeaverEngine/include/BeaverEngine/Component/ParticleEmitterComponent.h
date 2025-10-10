#pragma once
#include "BeaverEngine/Component/LogicComponent.h"
#include "BeaverEngine/Utils/Particle.h"
namespace bv
{
    class ParticleEmitterComponent
        : public Component
    {
    public:
        static constexpr auto type_ = "ParticleEmitter";


        ParticleEmitterComponent(Entity& owner) : Component(owner) {}
        ~ParticleEmitterComponent() = default;
        ParticleEmitterComponent(const ParticleEmitterComponent&) = default;
        ParticleEmitterComponent& operator=(const ParticleEmitterComponent&) = default;
        ParticleEmitterComponent(ParticleEmitterComponent&&) = default;
        ParticleEmitterComponent& operator=(ParticleEmitterComponent&&) = default;

        void setup(const ComponentDescription& init_value) override;

        void emit(unsigned int particle_count, const glm::vec3& direction/* (0,0,0) for burst*/);

    private:
        enum EmitterPrimitive
        {
            POINT, CIRCLE, SQUARE, FOUNTAIN
        };


        ParticleProps particle_properties_{};
        EmitterPrimitive primitive_{};

        float particle_velocity_{}, particle_velocity_variation_ = 0.f;

        const  glm::vec3 NULL_VEC{ 0.f,0.f,0.f };

        float primitive_parameter_{};
        bool emit_outer_ = false;
        bool emit_radial = true;

        std::map<std::string, EmitterPrimitive> string_to_primitive_map_ =
        {
            {"point" , POINT},
            {"circle", CIRCLE},
            {"square", SQUARE},
            {"fountain" , FOUNTAIN}
        };

        enum InitValue
        {
            START_BEGIN, END_COLOR, START_SIZE, END_SIZE, ANIMATION, LIFESPAN, LIFESPAN_VARIATION,
            PRIMITIVE, VELOCITY, VELOCITY_VARIATION, PRIMITIVE_PARAMETER, EMIT_OUTER, EMIT_RADIAL
        };
        const std::map<std::string_view, InitValue> string_to_init_enum_map_ = {
            {"start_color"   , START_BEGIN},
            {"end_color"     , END_COLOR},
            {"start_size"    , START_SIZE},
            {"end_size"      , END_SIZE},
            {"animation"     , ANIMATION},
            {"lifespan"      , LIFESPAN},
            {"lifespan_var"  , LIFESPAN_VARIATION},
            {"primitive"     , PRIMITIVE},
            {"velocity"      , VELOCITY},
            {"velocity_var"  , VELOCITY_VARIATION},
            {"circle_radius" , PRIMITIVE_PARAMETER},
            {"square_size"   , PRIMITIVE_PARAMETER},
            {"fountain_angle", PRIMITIVE_PARAMETER},
            {"emit_outer"    , EMIT_OUTER},
            {"emit_radial"   , EMIT_RADIAL}
        };

        void emitFountain(unsigned int particle_count, const glm::vec3& direction);

        void emitSquare(unsigned int particle_count, const glm::vec3& direction);

        void emitCircle(unsigned int particle_count, const glm::vec3& direction);

        void EmitPoint(unsigned int particle_count, const glm::vec3& direction);
    };
}
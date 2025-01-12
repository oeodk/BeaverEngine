#pragma once
#include "BeaverEngine/Utils/Texture.h"
#include <glm/glm.hpp>
namespace bv
{
    struct AnimationData
    {
        AnimationData(unsigned int i_width, unsigned int i_height, unsigned int i_frame_count, float i_frame_dt, const glm::vec2& i_start_coords)
            : width(i_width), height(i_height), frame_count(i_frame_count), frame_dt(i_frame_dt), start_coordinate(i_start_coords) {}
        unsigned int width, height;
        unsigned int frame_count;
        float frame_dt; //ms
        glm::vec2 start_coordinate;
    };

    class Texture2D :
        public Texture
    {
	public:
        ~Texture2D();
		static std::shared_ptr<Texture2D> create(const TextureSpecification& specification);
		static std::shared_ptr<Texture2D> create(std::string_view path, bool interpolate = false);

        glm::vec2 getSpriteSize(const std::string& sprite_name) const
        {
            if (texture_animations_.contains(sprite_name))
            {
                glm::vec2 tmp = glm::vec2(texture_animations_.at(sprite_name).width, texture_animations_.at(sprite_name).height);
                return glm::vec2(texture_animations_.at(sprite_name).width, texture_animations_.at(sprite_name).height);
            }
            return glm::vec2(getWidth(), getHeight());
        }

        glm::vec2 getSpriteCoordinate(const std::string& sprite_name, float& animation_dt) const
        {
            if (texture_animations_.contains(sprite_name))
            {
                const auto& data = texture_animations_.at(sprite_name);
                int current_frame = int(animation_dt / data.frame_dt);
                if (current_frame >= data.frame_count)
                {
                    current_frame = 0;
                    animation_dt = animation_dt - data.frame_count * data.frame_dt;
                }
                glm::vec2 out(data.start_coordinate);
                out.x += current_frame * data.width;
                return out;
            }
            return glm::vec2(0,0);
        }
    protected:
        std::map<std::string, AnimationData> texture_animations_;
    };
}


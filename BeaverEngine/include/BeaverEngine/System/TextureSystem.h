#pragma once
#include "BeaverEngine/System/System.h"
#include "BeaverEngine/Utils/Texture2D.h"
#include "BeaverEngine/Utils/Texture.h"
#include "BeaverEngine/Core/std.h"

namespace bv
{
    class TextureSystem :
        public System
    {
    public:
        TextureSystem() = default;
        TextureSystem(const TextureSystem&) = default;
        TextureSystem(TextureSystem&&) = default;
        TextureSystem& operator=(const TextureSystem&) = default;
        TextureSystem& operator=(TextureSystem&&) = default;

        ~TextureSystem() = default;

        static TextureSystem& getInstance();

        void iterate(const Timing& dt) override
        {
            for (auto& texture : texture_2d_to_check_)
            {
                if (textures_2d_.at(texture).expired())
                {
                    textures_2d_.erase(texture);
                }
            }
            texture_2d_to_check_.clear();
        }

        std::shared_ptr<Texture2D> getTexture2D(std::string_view path, bool interpolate = false)
        {
            std::string string_path(path);
            if (textures_2d_.contains(string_path))
            {
                return textures_2d_.at(string_path).lock();
            }
            std::shared_ptr<Texture2D> texture = Texture2D::create(path, interpolate);
            textures_2d_.insert({ string_path, texture });
            return texture;
        }

        void texture2dRemoved(std::string_view texture_name)
        {
            texture_2d_to_check_.insert(std::string(texture_name));
        }

    private:
        std::unordered_map<std::string, std::weak_ptr<Texture2D>> textures_2d_;
        std::set<std::string> texture_2d_to_check_;
    };
}


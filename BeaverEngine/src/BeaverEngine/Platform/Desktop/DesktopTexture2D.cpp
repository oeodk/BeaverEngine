#include "BeaverEngine/Platform/Desktop/DesktopTexture2D.h"
#include "deps/stb_image/stb_image.h"
#include "yaml-cpp/yaml.h"
namespace bv
{
	Texture2D::~Texture2D()
	{
	}
	std::shared_ptr<Texture2D> Texture2D::create(const TextureSpecification& specification)
	{
		return std::make_shared<DesktopTexture2D>(specification);
	}

	std::shared_ptr<Texture2D> Texture2D::create(std::string_view path, bool interpolate)
	{
		return std::make_shared<DesktopTexture2D>(path, interpolate);
	}

	static GLenum imageFormatToGLDataFormat(ImageFormat format)
	{
		switch (format)
		{
		case ImageFormat::RGB8:  return GL_RGB;
		case ImageFormat::RGBA8: return GL_RGBA;
		}

		return 0;
	}

	static GLenum imageFormatToGLInternalFormat(ImageFormat format)
	{
		switch (format)
		{
		case ImageFormat::RGB8:  return GL_RGB8;
		case ImageFormat::RGBA8: return GL_RGBA8;
		}
		return 0;
	}

	DesktopTexture2D::DesktopTexture2D(const TextureSpecification& specification)
		: specification_(specification), width_(specification.width), height_(specification.height)
	{
		international_format_ = imageFormatToGLInternalFormat(specification.format);
		data_format_ = imageFormatToGLDataFormat(specification.format);

		glCreateTextures(GL_TEXTURE_2D, 1, &texture_id_);
		glTextureStorage2D(texture_id_, 1, international_format_, width_, width_);

		glTextureParameteri(texture_id_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(texture_id_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(texture_id_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(texture_id_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	DesktopTexture2D::DesktopTexture2D(std::string_view path, bool interpolate)
	{
		path_ = std::string(path);
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			data = stbi_load(path.data(), &width, &height, &channels, 0);
		}

		if (data)
		{
			is_loaded_ = true;

			width_ = width;
			height_ = height;

			GLenum international_format = 0, data_format = 0;
			if (channels == 4)
			{
				international_format = GL_RGBA8;
				data_format = GL_RGBA;
			}
			else if (channels == 3)
			{
				international_format = GL_RGB8;
				data_format = GL_RGB;
			}

			international_format_ = international_format;
			data_format_ = data_format;

			GLint interpolation = GL_NEAREST;
			if (interpolate)
			{
				interpolation = GL_LINEAR;
			}

			_ASSERT(international_format & data_format, "Format not supported!");

			glCreateTextures(GL_TEXTURE_2D, 1, &texture_id_);
			//int mip_levels = 1 + std::floor(std::log2(std::max(width_, height_)));
			glTextureStorage2D(texture_id_, 1, international_format, width_, height_);

			glTextureParameteri(texture_id_, GL_TEXTURE_MIN_FILTER, interpolation);
			glTextureParameteri(texture_id_, GL_TEXTURE_MAG_FILTER, interpolation);

			glTextureParameteri(texture_id_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameteri(texture_id_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glTextureSubImage2D(texture_id_, 0, 0, 0, width_, height_, data_format, GL_UNSIGNED_BYTE, data);
			glGenerateTextureMipmap(texture_id_);

			stbi_image_free(data);

			size_t pos = path_.find_last_of('.');
			std::string data_path(path.substr(0, pos));
			data_path += +".yaml";
			if (std::filesystem::exists(data_path))
			{
				YAML::Node texture_data = YAML::LoadFile(data_path);
				for (const auto& sprite_data : texture_data)
				{
					texture_animations_.insert({ sprite_data["animation"].as<std::string>(), AnimationData(sprite_data["width"].as<unsigned int>(), sprite_data["height"].as<unsigned int>(), sprite_data["frame_count"].as<unsigned int>(), sprite_data["frame_dt"].as<float>(), glm::vec2(sprite_data["start_coordinates"][0].as<unsigned int>(), sprite_data["start_coordinates"][1].as<unsigned int>()))});
				}
			}
		}
	}

	DesktopTexture2D::~DesktopTexture2D()
	{
		glDeleteTextures(1, &texture_id_);
	}

	void DesktopTexture2D::setData(void* data, unsigned int size)
	{
		uint32_t bpp = data_format_ == GL_RGBA ? 4 : 3;
		_ASSERT(size == width_ * height_* bpp, "Data must be entire texture!");
		glTextureSubImage2D(texture_id_, 0, 0, 0, width_, height_, data_format_, GL_UNSIGNED_BYTE, data);
	}

	void DesktopTexture2D::bind(unsigned int slot) const
	{
		glBindTextureUnit(slot, texture_id_);
	}
}
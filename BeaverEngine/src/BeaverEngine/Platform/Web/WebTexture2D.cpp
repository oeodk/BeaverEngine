#include "BeaverEngine/Platform/PlatformMacros.h"
#ifdef PLATFORM_WEB

#include "BeaverEngine/Platform/Web/WebTexture2D.h"
#include "BeaverEngine/System/TextureSystem.h"

#include "BeaverEngine/Core/TypeDef.h"
#include "deps/stb_image/stb_image.h"
#include "yaml-cpp/yaml.h"
namespace bv
{
	std::shared_ptr<Texture2D> Texture2D::create(const TextureSpecification& specification)
	{
		return std::make_shared<WebTexture2D>(specification);
	}

	std::shared_ptr<Texture2D> Texture2D::create(std::string_view path, bool interpolate)
	{
		return std::make_shared<WebTexture2D>(path, interpolate);
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

	WebTexture2D::WebTexture2D(const TextureSpecification& specification)
		: specification_(specification), width_(specification.width), height_(specification.height)
	{
		international_format_ = imageFormatToGLInternalFormat(specification.format);
		data_format_ = imageFormatToGLDataFormat(specification.format);

		glGenTextures(1, &texture_id_);

		glBindTexture(GL_TEXTURE_2D, texture_id_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Configures the way the texture repeats (if it does at all)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(
				GL_TEXTURE_2D,
				0,                // mip level
				GL_RGBA,      // internal format (e.g. GL_RGBA)
				width_,
				height_,
				0,                // border must be 0
				GL_RGBA,      // same as above
				GL_UNSIGNED_BYTE,
				nullptr           // no initial data yet
			);
		if(specification.generate_mips)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}

	WebTexture2D::WebTexture2D(std::string_view path, bool interpolate)
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

			glGenTextures(1, &texture_id_);
	
			glBindTexture(GL_TEXTURE_2D, texture_id_);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolation);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolation);

			// Configures the way the texture repeats (if it does at all)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,                // mip level
				data_format,      // internal format (e.g. GL_RGBA)
				width_,
				height_,
				0,                // border must be 0
				data_format,      // same as above
				GL_UNSIGNED_BYTE,
				nullptr           // no initial data yet
			);
			
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_, data_format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			stbi_image_free(data);

			size_t pos = path_.find_last_of('.');
			std::string data_path(path.substr(0, pos));
			data_path += +".yaml";
			if (std::filesystem::exists(data_path))
			{
				Description texture_data = Descr::load(data_path);
				for (const auto& sprite_data : texture_data)
				{
					texture_animations_.insert({ sprite_data["animation"].as<std::string>(), AnimationData(sprite_data["width"].as<unsigned int>(), sprite_data["height"].as<unsigned int>(), sprite_data["frame_count"].as<unsigned int>(), sprite_data["frame_dt"].as<float>(), glm::vec2(sprite_data["start_coordinates"][0].as<unsigned int>(), sprite_data["start_coordinates"][1].as<unsigned int>()))});
				}
			}
		}
	}

	WebTexture2D::~WebTexture2D()
	{
		TextureSystem::getInstance().texture2dRemoved(getPath());

		glDeleteTextures(1, &texture_id_);
	}

	void WebTexture2D::setData(void* data, unsigned int size)
	{
		uint32_t bpp = data_format_ == GL_RGBA ? 4 : 3;
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_, data_format_, GL_UNSIGNED_BYTE, data);
	}

	void WebTexture2D::bind(unsigned int slot) const
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_id_);
	}
}

#endif
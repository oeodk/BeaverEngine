#pragma once
#include "BeaverEngine/Core/std.h"

namespace bv
{
	enum class ImageFormat
	{
		None = 0,
		R8,
		RGB8,
		RGBA8,
		RGBA32F
	};

	struct TextureSpecification
	{
		unsigned int width = 1;
		unsigned int height = 1;
		ImageFormat format = ImageFormat::RGBA8;
		bool generate_mips = true;
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual const TextureSpecification& getSpecification() const = 0;

		virtual unsigned int getWidth() const = 0;
		virtual unsigned int getHeight() const = 0;
		virtual unsigned int getRendererID() const = 0;

		virtual const std::string& getPath() const = 0;

		virtual void setData(void* data, uint32_t size) = 0;

		virtual void bind(unsigned int slot = 0) const = 0;

		virtual bool isLoaded() const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};
}


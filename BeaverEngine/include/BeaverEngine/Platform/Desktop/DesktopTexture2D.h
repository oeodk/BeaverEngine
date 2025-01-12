#pragma once
#include "BeaverEngine/Utils/Texture2D.h"
#include <glad/glad.h>

namespace bv
{
	class DesktopTexture2D :
		public Texture2D
	{
	public:
		DesktopTexture2D(const TextureSpecification& specification);
		DesktopTexture2D(std::string_view path, bool interpolate);
		virtual ~DesktopTexture2D();

		const TextureSpecification& getSpecification() const override { return specification_; }
		unsigned int getWidth() const override { return width_; }
		unsigned int getHeight() const override { return height_; }
		unsigned int getRendererID() const override { return texture_id_; }
		const std::string& getPath() const override { return path_; }
		void setData(void* data, unsigned int size) override;
		void bind(unsigned int slot = 0) const override;
		bool isLoaded() const override { return is_loaded_; }
		bool operator==(const Texture& other) const override { return texture_id_ == other.getRendererID(); }
	private:
		TextureSpecification specification_;

		std::string path_{};
		bool is_loaded_ = false;
		unsigned int width_{}, height_{};
		unsigned int texture_id_;

		GLenum international_format_, data_format_;;
	};
}


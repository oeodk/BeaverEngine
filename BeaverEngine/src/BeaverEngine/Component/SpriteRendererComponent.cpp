#include "BeaverEngine/Component/SpriteRendererComponent.h"
#include "BeaverEngine/Component/PositionComponent.h"

#include "BeaverEngine/Core/Entity.h"
#include "BeaverEngine/Core/Renderer.h"
#include "BeaverEngine/Core/Scene.h"
#include "BeaverEngine/Core/GlobalConstants.h"

#include "BeaverEngine/System/TextureSystem.h"
#include "BeaverEngine/System/ViewSystem.h"
#include "BeaverEngine/System/WindowSystem.h"
#include "BeaverEngine/System/EntitySystem.h"

#include "BeaverEngine/Utils/Window.h"

#include <glad/glad.h>
#include <glm/gtc/quaternion.hpp>

namespace bv
{
	void SpriteRendererComponent::setup(const ComponentDescription& init_value)
	{
		SpriteComponent::setup(init_value);

		std::weak_ptr<Window> main_window = WindowSystem::getInstance().getMainWindow();
		main_window.lock()->makeCurrent();
		bool init_window = true;
		bool init_view = true;

		for (auto& value : init_value.parameters)
		{
			if(string_to_init_enum_map_.contains(value.first))
			{
				switch (string_to_init_enum_map_.at(value.first))
				{
				case bv::SpriteRendererComponent::WINDOW:
					initWindow(value.second);
					init_window = false;
					break;
				case bv::SpriteRendererComponent::VIEW:
					initView(value.second);
					init_view = false;
					break;
				case bv::SpriteRendererComponent::TEXTURE:
				{
					bool interpolate = false;

					if (init_value.parameters.contains("interpolate_texture") && init_value.parameters.at("interpolate_texture").as<bool>() == true)
					{
						interpolate = true;
					}
					initTexture(value.second, interpolate);
				}
				break;
				}
			}
		}
		if (init_window)
		{
			window_to_render_ = WindowSystem::getInstance().getMainWindow();
		}
		if (init_view)
		{
			view_to_render_ = ViewSystem::getInstance().getMainView();
		}
		refreshPoint();
		setRenderRectangle(render_rect_);

		auto mapped_indices = index_buffer_.mapIndices(0, 6);
		std::copy(INDICES.begin(), INDICES.end(), mapped_indices.begin());
		index_buffer_.setup();

		updateMesh();
		vertex_buffer_.setup();
	}

	void SpriteRendererComponent::resolve()
	{
		if (!fixed_size_)
		{
			size_.x = texture_->getWidth();
			size_.y = texture_->getHeight();
		}
		if (init_render_rect_)
		{
			render_rect_.size.x = texture_->getWidth();
			render_rect_.size.y = texture_->getHeight();
		}

		refreshPoint();
		setRenderRectangle(render_rect_);
		updateMesh();
		refreshAnimation();

		index_buffer_.setup();

	}


	void SpriteRendererComponent::display(Renderer* renderer, const Timing& dt)
	{
		updateMesh();
		updadeVertexBuffer();
		texture_->bind();
		renderer->render(vertex_buffer_, index_buffer_, window_to_render_.lock().get(), view_to_render_);
	}

	void SpriteRendererComponent::initTexture(const Description& value, bool interpolate)
	{
		std::string file = value.as<std::string>();
		if (file.find("data") == std::string::npos)
		{
			texture_ = TextureSystem::getInstance().getTexture2D(constants::SPRITES_PATH + file, interpolate);
		}
		else
		{
			texture_ = TextureSystem::getInstance().getTexture2D(file, interpolate);
		}
	}

	void SpriteRendererComponent::initWindow(const Description& value)
	{

		window_to_render_ = WindowSystem::getInstance().getWindow(value.as<std::string>());
		if (window_to_render_.expired())
		{
			window_to_render_ = WindowSystem::getInstance().getMainWindow();
		}
	}

	void SpriteRendererComponent::initView(const Description& value)
	{
		view_to_render_ = ViewSystem::getInstance().getView(value.as<std::string>());

		if (view_to_render_ == nullptr)
		{
			view_to_render_ = ViewSystem::getInstance().getMainView();
		}
	}
	void SpriteRendererComponent::updadeVertexBuffer()
	{
		auto mapped_vertices = vertex_buffer_.mapVertices(0, 4);
		mapped_vertices[0] = { vertices_[0] };
		mapped_vertices[1] = { vertices_[1] };
		mapped_vertices[2] = { vertices_[2] };
		mapped_vertices[3] = { vertices_[3] };
		vertex_buffer_.setup();
	}
}
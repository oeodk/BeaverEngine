#include "BeaverEngine/Component/LayerComponent.h"

#include "BeaverEngine/Core/Entity.h"
#include "BeaverEngine/Core/Renderer.h"
#include "BeaverEngine/Core/GlobalConstants.h"

#include "BeaverEngine/Component/PositionComponent.h"
#include "BeaverEngine/Component/SpriteComponent.h"

#include "BeaverEngine/System/TextureSystem.h"
#include "BeaverEngine/System/ViewSystem.h"
#include "BeaverEngine/System/WindowSystem.h"
#include "BeaverEngine/System/EntitySystem.h"

namespace bv
{
	LayerComponent::~LayerComponent()
	{
		for (auto& sprite : sprites_)
		{
			sprite->setLayer(nullptr);
		}
	}

	void LayerComponent::setup(const ComponentDescription& init_value)
	{
		std::weak_ptr<Window> main_window = WindowSystem::getInstance().getMainWindow();
		main_window.lock()->makeCurrent();

		bool init_window = true;
		bool init_view = true;

		for (auto& value : init_value.parameters)
		{
			switch (string_to_init_enum_map_.at(value.first))
			{
			case LayerComponent::WINDOW:
				initWindow(value.second);
				init_window = false;
				break;
			case LayerComponent::VIEW:
				initView(value.second);
				init_view = false;
				break;
			case LayerComponent::TEXTURE:
			{
				bool interpolate = false;

				if (init_value.parameters.contains("interpolate_texture") && init_value.parameters.at("interpolate_texture").as<bool>() == true)
				{
					interpolate = true;
				}
				initTexture(value.second, interpolate);
			}
			break;
			case SORT:
				sort_sprites_ = value.second.as<bool>();
			break;
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
	}
	void LayerComponent::updateLogic(const Timing& timing)
	{
		if (window_to_render_.expired())
		{
			for (auto& sprite : sprites_)
			{
				EntitySystem::remove(sprite->owner().shared_from_this());
			}
			EntitySystem::remove(owner().shared_from_this());
		}
	}

	void addSpriteToLayer(int i, float layer_z, int sprite_count, std::vector<SpriteComponent*>& sorted_sprite, std::span<Vertex2D>& mapped_vertices, std::span<unsigned int>& mapped_indices)
	{
		sorted_sprite[i]->updateMesh();
		std::transform(sorted_sprite[i]->getVertices().begin(), sorted_sprite[i]->getVertices().end(), mapped_vertices.begin() + i * 4,
			[i, sprite_count, layer_z, &sorted_sprite](const Vertex2D& v) {
				Vertex2D vert(v);
				vert.position.z = layer_z + i / static_cast<float>(sprite_count);

				vert.color = sorted_sprite[i]->getColor();

				return vert; });
		std::transform(sorted_sprite[i]->getIndices().begin(), sorted_sprite[i]->getIndices().end(), mapped_indices.begin() + i * 6,
			[i](unsigned int index) {return index + i * 4; });
	}

	void LayerComponent::display(Renderer* renderer, const Timing& dt)
	{
		std::vector<SpriteComponent*> sorted_sprite;
		sorted_sprite.reserve(std::size(sprites_));
		if(sort_sprites_)
		{
			std::sort(std::execution::par, sprites_.begin(), sprites_.end(),
				[](const SpriteComponent* s1, const SpriteComponent* s2) {
					return SpriteComponent::compareSpritesPosition(s2, s1);
				});
		}
		std::copy_if(sprites_.begin(), sprites_.end(), std::back_inserter(sorted_sprite),
			[](const SpriteComponent* sprite) {
				return sprite->enabled() && sprite->owner().active() && sprite->willRender();
			});

		vertex_buffer_ = VertexBuffer<Vertex2D>();
		index_buffer_ = IndexBuffer();

		auto mapped_vertices = vertex_buffer_.mapVertices(0, 4 * sorted_sprite.size());
		auto mapped_indices = index_buffer_.mapIndices(0, 6 * sorted_sprite.size());

		const size_t sprite_count = std::size(sorted_sprite);
		float layer_z = owner().getComponent<PositionComponent>()->getWorldPosition().z;

		std::vector<std::future<void>> threads;
		threads.reserve(sprite_count);
		for (size_t i = 0; i < sprite_count; i++)
		{
			threads.push_back(std::async(std::launch::async, &addSpriteToLayer, i, layer_z, sprite_count, std::ref(sorted_sprite), std::ref(mapped_vertices), std::ref(mapped_indices)));
		}
		for (size_t i = 0; i < sprite_count; i++)
		{
			threads[i].wait();
		}
		vertex_buffer_.setup();
		index_buffer_.setup();

		texture_->bind();
		renderer->render(vertex_buffer_, index_buffer_, window_to_render_.lock().get(), view_to_render_);
		
	}


	void LayerComponent::initTexture(const Description& value, bool interpolate)
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
	
	void LayerComponent::initWindow(const Description& value)
	{
	
		window_to_render_ = WindowSystem::getInstance().getWindow(value.as<std::string>());
		if(window_to_render_.expired())
		{
			window_to_render_ = WindowSystem::getInstance().getMainWindow();
		}
	}
	
	void LayerComponent::initView(const Description& value)
	{
		view_to_render_ = ViewSystem::getInstance().getView(value.as<std::string>());
	
		if(view_to_render_ == nullptr)
		{
			view_to_render_ = ViewSystem::getInstance().getMainView();
		}
	}
}

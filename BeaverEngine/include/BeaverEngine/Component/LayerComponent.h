#pragma once
#include "BeaverEngine/Component/LogicComponent.h"
#include "BeaverEngine/Component/SpriteComponent.h"
#include "BeaverEngine/Component/DisplayComponent.h"
#include "BeaverEngine/Utils/Buffer.h"
#include "BeaverEngine/Utils/Texture2D.h"
#include "BeaverEngine/Utils/Vertex.h"

namespace bv
{
	class View2D;

	class LayerComponent
		: public LogicComponent, public DisplayComponent
	{
	public:
		static constexpr auto type_ = "Layer";

		LayerComponent(Entity& owner) : Component(owner) { dimension_ = RenderDimension::RENDER_2D; }
		~LayerComponent();
		LayerComponent(const LayerComponent&) = default;
		LayerComponent& operator=(const LayerComponent&) = default;
		LayerComponent(LayerComponent&&) = default;
		LayerComponent& operator=(LayerComponent&&) = default;

		void setup(const ComponentDescription& init_value) override;

		void updateLogic(const Timing& timing) override;
		void display(Renderer* renderer, const Timing& dt) override;
		
		void addSprite(SpriteComponent* sprite) { sprites_.push_back(sprite); }
		void removeSprite(SpriteComponent* sprite) { sprites_.erase(std::remove(sprites_.begin(), sprites_.end(), sprite), sprites_.end()); }

		const std::weak_ptr<Texture2D> getTexture() const { return texture_; }
	private:
		
		

		std::vector<SpriteComponent*> sprites_;

		std::shared_ptr<Texture2D> texture_;

		VertexBuffer<Vertex2D> vertex_buffer_;
		IndexBuffer index_buffer_;
		View2D* view_to_render_{};

		void initTexture(const Description& value, bool interpolate);
		void initWindow(const Description& value);
		void initView(const Description& value);

		enum InitValue
		{
			WINDOW, VIEW, TEXTURE, INTERPOLATE_TEXTURE
		};
		const std::map<std::string_view, InitValue> string_to_init_enum_map_ = {
			{"window" , WINDOW},
			{"view"   , VIEW},			
			{"interpolateTexture", INTERPOLATE_TEXTURE},
			{"texture", TEXTURE}
		};
	};
}


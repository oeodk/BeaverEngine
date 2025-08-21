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

	class SpriteRendererComponent
		: public DisplayComponent, public SpriteComponent
	{
	public:
		static constexpr auto type_ = "SpriteRenderer";

		SpriteRendererComponent(Entity& owner) : SpriteComponent(owner), Component(owner) { dimension_ = RenderDimension::RENDER_2D; }
		void setup(const ComponentDescription& init_value) override;
		void resolve() override;
		virtual void display(Renderer* renderer, const Timing& dt) override;

	private:

		
		std::shared_ptr<Texture2D> texture_;

		VertexBuffer<Vertex2D> vertex_buffer_;
		IndexBuffer index_buffer_{};
		View2D* view_to_render_{};

		enum E_InitValue
		{
			WINDOW, VIEW, TEXTURE, INTERPOLATE_TEXTURE
		};
		const std::map<std::string, E_InitValue> string_to_init_enum_map_ = {
			{"window"             , WINDOW},
			{"view"               , VIEW},
			{"texture"            , TEXTURE},
			{"interpolate_texture", INTERPOLATE_TEXTURE},
		};

		void initTexture(const Description& value, bool interpolate);
		void initWindow(const Description& value);
		void initView(const Description& value);

		void updadeVertexBuffer();

		const std::shared_ptr<Texture2D> getTexture() const override { return texture_; }

	};
}


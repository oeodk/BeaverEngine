#pragma once
#include "BeaverEngine/Component/LogicComponent.h"
#include "BeaverEngine/Component/DisplayComponent.h"
#include "BeaverEngine/Utils/Buffer.h"
#include "BeaverEngine/Utils/Vertex.h"
#include "BeaverEngine/Utils/Texture2D.h"
#include "BeaverEngine/Utils/Math.h"
namespace bv
{
	class View2D;

	class SpriteComponent
		: public LogicComponent, public DisplayComponent
	{
	public:
		static constexpr auto type_ = "Sprite";

		SpriteComponent(Entity& owner) : Component(owner) { dimension_ = E_RenderDimension::RENDER_2D; }
		void setup(const ComponentDescription& init_value) override;


		virtual void updateLogic(const Timing& timing) override;
		virtual void display(Renderer* renderer, const Timing& dt) override;

		void updateAnimation(const Timing& timing);
		void updateMesh();

		void setColor(const glm::vec4& color) { color_ = color; }
		const glm::vec4& getColor() const { return color_; }

		void setSize(const glm::vec2& size)
		{ 
			size_ = size; 
			refreshPoint();
		}
		const glm::vec2& getSize() const { return size_; }

		void setScale(const glm::vec2& scale)
		{ 
			scale_ = scale;
			refreshPoint();
		}
		const glm::vec2& getScale() const { return scale_; }

		//Angle in degree
		void setRotationAngle(float angle)
		{
			rotation_angle_ = angle;
			refreshPoint();
		}
		//Angle in degree
		float getRotationAngle() const { return rotation_angle_; }

		void setOffset(const glm::vec2& offset) { offset_ = glm::vec3(offset,0); }
		const glm::vec2& getOffset() const { return offset_; }

		void setRenderRectangle(const FloatRect& render_rect);
		
		void setAnimationName(std::string_view new_name) { animation_name_ = new_name; }
		float resetAnimationDt(float value) { animation_dt_ = 0; }
	private:

		glm::vec4 color_{ 1,1,1,1 };
		glm::vec2 size_{};
		glm::vec2 scale_{1,1};
		glm::vec3 offset_{};
		float rotation_angle_{};
		FloatRect render_rect_{glm::vec2(), glm::vec2(1,1)};

		bool fixed_size_{};

		std::string animation_name_ = "";
		float animation_dt_{};

		std::array<glm::vec3, 4> points_{};
		std::array<glm::vec2, 4> texture_coords_{};

		static constexpr std::array<unsigned int, 6> INDICES = { 0,1,2,0,2,3 };

		std::shared_ptr<Texture2D> texture_;

		VertexBuffer<Vertex2D> vertex_buffer_;
		IndexBuffer index_buffer_;
		View2D* view_to_render_;

		enum E_InitValue
		{
			SIZE, WINDOW, SCALE, VIEW, OFFSET, TEXTURE, COLOR, RENDER_RECT, INTERPOLATE_TEXTURE, ROTATION_ANGLE, ANIMATION
		};
		const std::map<std::string, E_InitValue> string_to_init_enum_map_ = { 
			{"size"               , SIZE},
			{"window"             , WINDOW},
			{"scale"              , SCALE},
			{"view"               , VIEW},
			{"offset"             , OFFSET},
			{"texture"            , TEXTURE},
			{"color"              , COLOR},
			{"render_rectangle"   , RENDER_RECT},
			{"rotation_angle"     , ROTATION_ANGLE},
			{"interpolate_texture", INTERPOLATE_TEXTURE},
			{"base_animation"     , ANIMATION}
		};

		void refreshPoint();

		void initColor(const Description& value);
		void initSize(const Description & value);
		void initOffset(const Description& value);
		void initScale(const Description& value);
		void initRenderRect(const Description& value);
		void initTexture(const Description& value, bool interpolate);
		void initWindow(const Description& value);
		void initView(const Description& value);
		void initAngle(const Description& value);
		void initAnimation(const Description& value);
	};
}


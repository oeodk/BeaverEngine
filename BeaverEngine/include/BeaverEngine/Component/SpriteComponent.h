#pragma once
#include "BeaverEngine/Component/LogicComponent.h"

#include "BeaverEngine/Utils/Math.h"
#include "BeaverEngine/Utils/Vertex.h"

namespace bv
{
	class LayerComponent;
	class SpriteComponent
		: public LogicComponent
	{
	public:
		static constexpr auto type_ = "Sprite";

		SpriteComponent(Entity& owner) : Component(owner) {}
		~SpriteComponent();
		void setup(const ComponentDescription& init_value) override;
		void resolve() override;

		void updateLogic(const Timing& timing) override;

		void updateAnimation(const Timing& timing);
		void updateMesh();

		void setColor(const glm::vec4& color) { color_ = color; }
		const glm::vec4& getColor() const { 
			return color_; }

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

		// the new layer need to use the same spritesheet as the previous layer, or change animation accordingly
		void setLayer(LayerComponent* layer) { layer_ = layer; }

		// For internal use
		const std::array<unsigned int, 6>& getIndices() const { return INDICES; }
		// For internal use
		const std::array<Vertex2D, 4>& getVertices() const { return vertices_; }
	private:

		std::string layer_name{};

		glm::vec4 color_{ 1,1,1,1 };
		glm::vec2 size_{};
		glm::vec2 scale_{1,1};
		glm::vec3 offset_{};
		float rotation_angle_{};
		FloatRect render_rect_{glm::vec2(), glm::vec2(1,1)};

		bool fixed_size_{};
		bool init_render_rect_ = true;

		std::string animation_name_ = "";
		float animation_dt_{};

		std::array<glm::vec3, 4> points_{};
		std::array<glm::vec2, 4> texture_coords_{};

		std::array<Vertex2D, 4> vertices_{};

		static constexpr std::array<unsigned int, 6> INDICES = { 0,1,2,0,2,3 };

		LayerComponent* layer_{};

		enum InitValue
		{
			SIZE, SCALE, LAYER, OFFSET, COLOR, RENDER_RECT, ROTATION_ANGLE, ANIMATION
		};
		const std::map<std::string_view, InitValue> string_to_init_enum_map_ = { 
			{"size"               , SIZE},
			{"layer"              , LAYER},
			{"scale"              , SCALE},
			{"offset"             , OFFSET},
			{"color"              , COLOR},
			{"render_rectangle"   , RENDER_RECT},
			{"rotation_angle"     , ROTATION_ANGLE},
			{"base_animation"     , ANIMATION}
		};

		void refreshPoint();

		void initColor(const Description& value);
		void initSize(const Description & value);
		void initOffset(const Description& value);
		void initScale(const Description& value);
		void initRenderRect(const Description& value);
		void initAngle(const Description& value);
		void initLayer(const Description& value);
		void initAnimation(const Description& value);
	};
}


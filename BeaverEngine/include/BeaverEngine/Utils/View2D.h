#pragma once
#include "BeaverEngine/Core/std.h"
#include "BeaverEngine/Core/TypeDef.h"
#include <glm/glm.hpp>
#include "yaml-cpp/yaml.h"

namespace bv
{
	// maybe inherit from a view or camera class (to support 3D)
	class View2D
	{
	public:
		View2D();
		View2D(const View2D&) = default;
		View2D(View2D&&) = default;
		View2D& operator=(const View2D&) = default;
		View2D& operator=(View2D&&) = default;

		~View2D() = default;

		void setupParameter(std::string_view parameter_name, const Description& descr);

		// Setters
		void setCenter(float x, float y);
		void setCenter(const glm::vec2& center){setCenter(center.x, center.y); }
		void setElevation(unsigned int Elevation);

		void setSize(float width, float height);
		void setSize(const glm::vec2& size) { setSize(size.x, size.y); };
		void setViewPort(const glm::vec2& pos, const glm::vec2& size);

		// Rotation around the look at axis (in degree)
		void setRotation(float angle);

		//Getters
		const glm::vec2& getCenter() const { return center_; }
		unsigned int getElevation() const { return elevation_; }

		const glm::vec2& getSize() const { return size_; }
		float getWidth() const { return size_.x; }
		float getHeight() const { return size_.y; }

		const glm::vec2& getViewPortPos() const { return viewport_pos_; }
		const glm::vec2& getViewPortSize() const { return viewport_size_; }

		// Rotation around the look at axis (in degree)
		float getRotation() const { return rotation_angle_; }
		
		const glm::mat4& getTransform();

		float getRadius() const { return radius_; }

		glm::vec2 normalizedWindowToViewCoordinate(const glm::vec2& pos)
		{
			glm::vec2 view_pos(pos);
			view_pos.y = 1 - view_pos.y;
			view_pos *= 2;
			view_pos.x -= 1;
			view_pos.y -= 1;
			view_pos = inverse_transform_ * glm::vec4(view_pos, 0, 0);
			view_pos += center_;
			return view_pos;
		}

	private:
		const glm::mat4& getView();
		const glm::mat4& getProjection();

		glm::mat4 transform_{};
		glm::mat4 inverse_transform_{};
		glm::mat4 view_{};
		glm::mat4 projection_{};
		bool update_transform_{ true };
		bool update_view_{ true };
		bool update_projection_{ true };

		glm::vec2 size_{};
		float radius_{};

		glm::vec2 center_{0,0};
		float elevation_{100};

		float rotation_angle_{};

		glm::vec2 viewport_pos_{0,0};
		glm::vec2 viewport_size_{1,1};

		enum E_IitialValue
		{
			SIZE, CENTER, ELEVATION, ROTATION_ANGLE, VIEWPORT_POS, VIEWPORT_SIZE, E_END
		};
		inline static const std::map<std::string, E_IitialValue> INITIAL_VALUE_MAP{
			{"size", SIZE},
			{"center", CENTER},
			{"elevation", ELEVATION},
			{"rotationAngle", ROTATION_ANGLE},
			{"viewportPosition", VIEWPORT_POS},
			{"viewportSize", VIEWPORT_SIZE}
		};
	};
}


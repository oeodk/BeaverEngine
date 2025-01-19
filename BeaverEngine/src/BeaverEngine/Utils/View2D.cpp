#include "BeaverEngine/Utils/View2D.h"
#include "BeaverEngine/Component/WindowManagerComponent.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>


namespace bv
{
	View2D::View2D()
	{
		setSize(WindowManagerComponent::getBaseWindowSize());
	}

	void View2D::setupParameter(std::string_view parameter_name, const Description& descr)
	{

		switch (INITIAL_VALUE_MAP.at(std::string(parameter_name)))
		{
		case bv::View2D::SIZE:
		{
			setSize(descr[0].as<float>(), descr[1].as<float>());
		}
		break;
		case bv::View2D::CENTER:
		{
			setCenter(descr[0].as<float>(), descr[1].as<float>());
		}
		break;
		case bv::View2D::ELEVATION:
			setElevation(descr.as<float>());
			break;
		case bv::View2D::ROTATION_ANGLE:
			setRotation(descr.as<float>());
			break;
		case bv::View2D::VIEWPORT_POS:
		{
			setViewPort(glm::vec2(descr[0].as<float>(), descr[1].as<float>()), getViewPortSize());
		}
		break;
		case bv::View2D::VIEWPORT_SIZE:
		{
			setViewPort(getViewPortPos(), glm::vec2(descr[0].as<float>(), descr[1].as<float>()));
		}
		break;
		}
		
	}

	void View2D::setCenter(float x, float y)
	{
		center_.x = x;
		center_.y = y;
		update_view_ = true;
	}

	void View2D::setElevation(unsigned int elevation)
	{
		elevation_ = elevation;
		update_view_ = true;
		update_projection_ = true;
	}

	void View2D::setSize(float width, float height)
	{
		size_.x = width;
		size_.y = height;
		update_projection_ = true;
	}

	void View2D::setViewPort(const glm::vec2& pos, const glm::vec2& size)
	{
		viewport_pos_ = pos;
		viewport_size_ = size;
	}

	void View2D::setRotation(float angle)
	{
		rotation_angle_ = angle;
		update_view_ = true;
	}

	const glm::mat4& View2D::getTransform()
	{
		if (update_transform_)
		{
			transform_ = getProjection() * getView();
			inverse_transform_ = glm::inverse(transform_);
			update_transform_ = false;
		}
		return transform_;
	}
	const glm::mat4& View2D::getView()
	{
		if (update_view_)
		{
			glm::vec3 up_dir(0.0f, 1.0f, 0.0f); //Up dir pointing to +y
			const glm::vec3 axis(0.0f, 0.0f, 1.0f); //Rotation around the z axis
			const float angle = glm::radians(rotation_angle_);

			const glm::quat rotation = glm::angleAxis(angle, glm::normalize(axis));
			up_dir = rotation * up_dir;

			view_ = glm::lookAt(glm::vec3(center_, elevation_), glm::vec3(center_, 0), up_dir);

			update_view_ = false;
			update_transform_ = true;
		}
		return view_;
	}
	const glm::mat4& View2D::getProjection()
	{
		if (update_projection_)
		{
			projection_ = glm::ortho(- size_.x / 2.f, size_.x / 2.f, -size_.y / 2.f, size_.y / 2.f, 0.f, elevation_ + 0.01f);
		
			update_projection_ = false;
			update_transform_ = true;
		}
		return projection_;
	}
}

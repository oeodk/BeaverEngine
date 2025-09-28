#include "BeaverEngine/Component/CircleColliderComponent.h"
#include "BeaverEngine/Component/RectangleColliderComponent.h"
#include "BeaverEngine/Component/IntGridColliderComponent.h"
#include "BeaverEngine/Component/PositionComponent.h"

#include "BeaverEngine/Core/Entity.h"

#include "BeaverEngine/Functions/Math.h"

#include <glm/gtc/quaternion.hpp>

namespace bv
{

	void CircleColliderComponent::setup(const ComponentDescription& init_value)
	{
		handler_type_ = init_value.parameters.at("handler").as<std::string>();
		flag_ = init_value.parameters.at("flag").as<int>();
		mask_ = init_value.parameters.at("mask").as<int>();
		radius_ = init_value.parameters.at("radius").as<float>();

		if (init_value.parameters.contains("offset"))
		{
			offset_.x = init_value.parameters.at("offset")[0].as<float>();
			offset_.y = init_value.parameters.at("offset")[1].as<float>();
		}

		if (init_value.parameters.contains("rotation_angle"))
		{
			rotation_angle_ = init_value.parameters.at("rotation_angle").as<float>();
		}
		setRotationAngle(rotation_angle_);
	}

	void CircleColliderComponent::setRotationAngle(float angle)
	{
		rotation_angle_ = angle;

		const glm::vec3 axis(0.0f, 0.0f, 1.0f); //Rotation around the z axis
		const float radians_angle = glm::radians(rotation_angle_);
		
		const glm::quat rotation = glm::angleAxis(radians_angle, glm::normalize(axis));
		
		true_center_ = glm::vec2(rotation * glm::vec3(offset_, 0));;
	}

	bool CircleColliderComponent::collides(const RectangleColliderComponent& other) const
	{
		return other.collides(*this);
	}

	bool CircleColliderComponent::collides(const CircleColliderComponent& other) const
	{
		const glm::vec2& position = glm::vec2(owner().getComponent<PositionComponent>()->getWorldPosition()) + true_center_;
		const glm::vec2& other_position = glm::vec2(other.owner().getComponent<PositionComponent>()->getWorldPosition()) + other.true_center_;
		const float dist = length2(position - other_position);
		return (radius_ + other.radius_) * (radius_ + other.radius_) > dist;
	}
	bool CircleColliderComponent::collides(const IntGridColliderComponent& other) const
	{
		return other.collides(*this);
	}

	bool CircleColliderComponent::collidesWithPoint(const glm::vec2& other) const
	{
		const glm::vec2& position = glm::vec2(owner().getComponent<PositionComponent>()->getWorldPosition()) + true_center_;
		const float dist = length2(position - other);
		return radius_ * radius_ > dist;
	}
}

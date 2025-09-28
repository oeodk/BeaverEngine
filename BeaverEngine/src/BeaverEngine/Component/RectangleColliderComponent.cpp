#include "BeaverEngine/Component/RectangleColliderComponent.h"
#include "BeaverEngine/Component/CircleColliderComponent.h"
#include "BeaverEngine/Component/IntGridColliderComponent.h"
#include "BeaverEngine/Component/PositionComponent.h"

#include "BeaverEngine/Core/Entity.h"
#include "BeaverEngine/Functions/Math.h"
#include <glm/gtc/quaternion.hpp>


namespace bv
{
	void RectangleColliderComponent::setup(const ComponentDescription& init_value)
	{
		handler_type_ = init_value.parameters.at("handler").as<std::string>();
		flag_ = init_value.parameters.at("flag").as<int>();
		mask_ = init_value.parameters.at("mask").as<int>();
		half_size_.x = init_value.parameters.at("size")[0].as<float>() / 2.f;
		half_size_.y = init_value.parameters.at("size")[1].as<float>() / 2.f;

		if (init_value.parameters.contains("offset"))
		{
			offset_.x = init_value.parameters.at("offset")[0].as<float>();
			offset_.y = init_value.parameters.at("offset")[1].as<float>();
		}

		if (init_value.parameters.contains("rotationAngle"))
		{
			rotation_angle_ = init_value.parameters.at("rotationAngle").as<float>();
		}
		setRotationAngle(rotation_angle_);
	}

	void RectangleColliderComponent::setRotationAngle(float angle)
	{
		rotation_angle_ = angle;

		points_[0] = -half_size_ + offset_;
		
		points_[1].x = -half_size_.x + offset_.x;
		points_[1].y =  half_size_.y + offset_.y;
		
		points_[2] = half_size_ + offset_;

		points_[3].x =  half_size_.x + offset_.x;
		points_[3].y = -half_size_.y + offset_.y;

		const float angle_rad = glm::radians(angle);

		glm::vec3 axis(0.0f, 0.0f, 1.0f);

		glm::quat rotation = glm::angleAxis(angle_rad, glm::normalize(axis));
		for (auto& point : points_)
		{
			point = rotation * glm::vec3(point, 0);
		}

		std::array<glm::vec2, 2> edge_vector;

		edge_vector[0] = points_[1] - points_[0];
		edge_vector[1] = points_[2] - points_[1];

		rectangles_perpendicular_edge_vector_[0] = glm::vec2(-edge_vector[0].y, edge_vector[0].x);
		rectangles_perpendicular_edge_vector_[1] = glm::vec2(-edge_vector[1].y, edge_vector[1].x);
	}


	bool RectangleColliderComponent::collides(const RectangleColliderComponent& other) const
	{
		const glm::vec2 center = glm::vec2(owner().getComponent<PositionComponent>()->getWorldPosition());
		const glm::vec2 other_center = glm::vec2(other.owner().getComponent<PositionComponent>()->getWorldPosition());

		if (rotation_angle_ == 0 && other.rotation_angle_ == 0)
		{
			const glm::vec2 dist = (center + offset_) - (other_center + other.offset_);
			if (std::abs(dist.x) < half_size_.x + other.half_size_.x && std::abs(dist.y) < half_size_.y + other.half_size_.y)
			{
				return true;
			}
			else
			{
				return false;
			}

		}
		else
		{
			float amin, amax, bmin, bmax;
			float point;
			std::array<const glm::vec2*, 4> rectangles_perpendicular_edge_vector_ref =
			{
				&rectangles_perpendicular_edge_vector_[0],
				&rectangles_perpendicular_edge_vector_[1],
				&other.rectangles_perpendicular_edge_vector_[0],
				&other.rectangles_perpendicular_edge_vector_[1]
			};

			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					point = glm::dot(*rectangles_perpendicular_edge_vector_ref[i], (points_[j] + center));
					if (j == 0)
					{
						amax = point;
						amin = point;
					}
					else
					{
						if (point > amax)
						{
							amax = point;
						}
						if (point < amin)
						{
							amin = point;
						}
					}
				}

				for (int j = 0; j < 4; j++)
				{
					point = glm::dot(*rectangles_perpendicular_edge_vector_ref[i], (other.points_[j] + other_center));
					if (j == 0)
					{
						bmax = point;
						bmin = point;
					}
					else
					{
						if (point > bmax)
						{
							bmax = point;
						}
						if (point < bmin)
						{
							bmin = point;
						}
					}
				}
				if (!((amin <= bmax && amin >= bmin) || (bmin <= amax && bmin >= amin)))
				{
					return false;
				}
			}
			return true;
		}
	}

	bool RectangleColliderComponent::collides(const CircleColliderComponent& other) const
	{
		const glm::vec2 circle_center = glm::vec2(other.owner().getComponent<PositionComponent>()->getWorldPosition()) + other.true_center_;
		
		const glm::vec3 axis(0.0f, 0.0f, 1.0f);

		const float angle = glm::radians(rotation_angle_);
		const glm::quat rotation = glm::angleAxis(angle, glm::normalize(axis));

		const glm::vec2 rectangle_center = glm::vec2(owner().getComponent<PositionComponent>()->getWorldPosition() + rotation * glm::vec3(offset_, 0));

		const float inverse_angle = glm::radians(-rotation_angle_);
		const glm::quat inverse_rotation = glm::angleAxis(inverse_angle, glm::normalize(axis));
		glm::vec2 dist = glm::vec2(inverse_rotation * glm::vec3(circle_center - rectangle_center, 0));
		dist.x = std::abs(dist.x);
		dist.y = std::abs(dist.y);

		if (dist.x > (half_size_.x + other.radius_))
		{
			return false;
		}

		if (dist.y > (half_size_.y + other.radius_))
		{
			return false;
		}
		if (dist.x <= (half_size_.x))
		{
			return true;
		}
		if (dist.y <= (half_size_.y))
		{
			return true;
		}
		float corner_dist_sq = length2(dist - half_size_);
		return (corner_dist_sq <= other.radius_ * other.radius_);
	}
	bool RectangleColliderComponent::collides(const IntGridColliderComponent& other) const
	{
		return other.collides(*this);
	}

	bool RectangleColliderComponent::collidesWithPoint(const glm::vec2& other) const
	{
		const glm::vec2 center = glm::vec2(owner().getComponent<PositionComponent>()->getWorldPosition());

		if (rotation_angle_ == 0)
		{
			const glm::vec2 dist = (center + offset_) - other;
			if (std::abs(dist.x) < half_size_.x&& std::abs(dist.y) < half_size_.y)
			{
				return true;
			}
			else
			{
				return false;
			}

		}
		else
		{
			float amin, amax, bmin, bmax;
			float point;
			std::array<const glm::vec2*, 2> rectangles_perpendicular_edge_vector_ref =
			{
				&rectangles_perpendicular_edge_vector_[0],
				&rectangles_perpendicular_edge_vector_[1]
			};

			for (int i = 0; i < rectangles_perpendicular_edge_vector_ref.size(); i++)
			{
				// Projection of this rectangle corner on the edge normals
				for (int j = 0; j < 4; j++)
				{
					point = glm::dot(*rectangles_perpendicular_edge_vector_ref[i], (points_[j] + center));
					if (j == 0)
					{
						amax = point;
						amin = point;
					}
					else
					{
						if (point > amax)
						{
							amax = point;
						}
						if (point < amin)
						{
							amin = point;
						}
					}
				}

				// Projection of the point to test on the rectangle edge normals
				point = glm::dot(*rectangles_perpendicular_edge_vector_ref[i], other);
				bmax = point;
				bmin = point;
					
				if (!((amin <= bmax && amin >= bmin) || (bmin <= amax && bmin >= amin)))
				{
					return false;
				}
			}
			return true;
		}
	}
}

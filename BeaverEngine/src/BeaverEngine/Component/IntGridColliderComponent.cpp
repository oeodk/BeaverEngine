#include "BeaverEngine/Component/IntGridColliderComponent.h"
#include "BeaverEngine/Component/CircleColliderComponent.h"
#include "BeaverEngine/Component/RectangleColliderComponent.h"
#include "BeaverEngine/Component/PositionComponent.h"

#include "BeaverEngine/Core/Entity.h"
#include "BeaverEngine/Core/GlobalConstants.h"

#include "BeaverEngine/Functions/Math.h"

#include <glm/gtc/quaternion.hpp>

namespace bv
{
	std::tuple<unsigned int, unsigned int> getGridDimension(std::string_view path, char separator)
	{
		std::fstream file(path.data());
		unsigned int width = 0;
		unsigned int height = 1;

		std::string tmp;
		std::getline(file, tmp);
		std::stringstream line(tmp);
		while (std::getline(line, tmp, separator))
		{
			width++;
		}
		while (std::getline(file, tmp))
		{
			height++;
		}
		return { width, height };
	}

	void bv::IntGridColliderComponent::setup(const ComponentDescription& init_value)
	{
		handler_type_ =  init_value.parameters.at("handler").as<std::string>();
		flag_ = init_value.parameters.at("flag").as<int>();
		mask_ = init_value.parameters.at("mask").as<int>();

		grid_size_ = init_value.parameters.at("gridSize").as<unsigned int>();
		char separator = init_value.parameters.at("separator").as<char>();

		std::string path = constants::LEVELS_PATH + init_value.parameters.at("filePath").as<std::string>();
		auto [width, height] = getGridDimension(path, separator);

		int_grid_.setup(width, height);

		std::fstream file(path.data());
		std::string line;

		size_t i = 0, j = 0;

		while (std::getline(file, line))
		{
			i = 0;
			std::string value;
			std::stringstream stream(line);
			while (std::getline(stream, value, separator))
			{
				int_grid_.set(i, j, std::stoi(value));
				i++;
			}
			j++;
		}

		offset_.x = (width) * grid_size_ / 2.f;
		offset_.y = (height) * grid_size_ / 2.f;
	}


	bool IntGridColliderComponent::testCollision(const std::array<glm::vec2, 4>& dp, const glm::vec2& other_position, int other_mask, const ColliderComponent* other) const
	{
		bool collide = false;
		glm::vec2 penetration(0,0);
		for (size_t i = 0; i < std::size(dp); i++)
		{
			glm::ivec2 int_coord(other_position + dp[i]);
			if (int_coord.x < 0 || int_coord.x > offset_.x * 2 || int_coord.y < 0 || int_coord.y > offset_.y * 2)
			{
				continue;
			}
			glm::uvec2 grid_coord;
			grid_coord.x = int_coord.x / static_cast<int>(grid_size_);
			grid_coord.y = int_coord.y / static_cast<int>(grid_size_);
			if (int_grid_.exist(grid_coord.x, grid_coord.y) && (int_grid_.at(grid_coord.x, grid_coord.y) & other_mask))
			{
				glm::vec2 dir(-dp[i]);
				int sx = sign(dir.x);
				int sy = sign(dir.y);

				bool grid_dx_empty = false;
				bool grid_dy_empty = false;
				if (int_grid_.exist(grid_coord.x + sx, grid_coord.y))
				{
					grid_dx_empty = !(int_grid_.at(grid_coord.x + sx, grid_coord.y) & other_mask);
				}
				if (int_grid_.exist(grid_coord.x, grid_coord.y + sy))
				{
					grid_dy_empty = !(int_grid_.at(grid_coord.x, grid_coord.y + sy) & other_mask);
				}
				if (grid_dx_empty && grid_dy_empty)
				{
					int int_grid_size_x = static_cast<int>(grid_size_) * ((sx + 1) * 0.5);
					int dpx = int_grid_size_x - sx * (int_coord.x - grid_coord.x * grid_size_);
					
					int int_grid_size_y = static_cast<int>(grid_size_) * ((sy + 1) * 0.5);
					int dpy = int_grid_size_y - sy * (int_coord.y - grid_coord.y * grid_size_);
					
					if (abs(dpx) < abs(dpy))
					{
						if (abs(penetration.x) < abs(dpx))
						{
							penetration.x = dpx * sx;
						}
					}
					else
					{
						if (abs(penetration.y) < abs(dpy))
						{
							penetration.y = -dpy * sy;
						}
					}
				}
				else
				{
					if (grid_dx_empty)
					{
						int int_grid_size = static_cast<int>(grid_size_) * ((sx + 1) * 0.5);
						int dp = int_grid_size - sx * (int_coord.x - grid_coord.x * grid_size_);
						if (abs(penetration.x) < abs(dp))
						{
							penetration.x = dp * sx;
						}
					}

					if (grid_dy_empty)
					{
						int int_grid_size = static_cast<int>(grid_size_) * ((sy + 1) * 0.5);
						int dp = int_grid_size - sy * (int_coord.y - grid_coord.y * grid_size_);
						if (abs(penetration.y) < abs(dp))
						{
							penetration.y = -dp * sy;
						}
					}
				}
				collide = true;
			}
		}
		if (collide)
		{
			collider_tile_map_.insert({ other, penetration });
			return true;
		}
		return false;
	}

	bool IntGridColliderComponent::collides(const RectangleColliderComponent& other) const
	{
		glm::vec2 other_position = other.owner().getComponent<PositionComponent>()->getRelativePosition();
		other_position += offset_;
		other_position.y = offset_.y * 2 - other_position.y - 2 * other.offset_.y;
		return testCollision(other.points_, other_position, other.mask_, &other);
	}

	bool IntGridColliderComponent::collides(const CircleColliderComponent& other) const
	{
		glm::vec2 other_position = other.owner().getComponent<PositionComponent>()->getRelativePosition();
		other_position += other.true_center_;
		other_position += offset_;
		other_position.y = offset_.y * 2 - other_position.y;

		const std::array<glm::vec2, 4> dp = { glm::vec2(other.radius_,0), glm::vec2(-other.radius_,0), glm::vec2(0,other.radius_), glm::vec2(0,-other.radius_) };
		return testCollision(dp, other_position, other.mask_, &other);
	}
	
}

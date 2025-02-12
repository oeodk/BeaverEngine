#pragma once
#include <glm/glm.hpp>

namespace bv
{
	struct CollisionData
	{
		bool collision{};
		glm::vec3 collision_point;
		float interpenetration_{};
		glm::vec3 collision_normal;
	};
}
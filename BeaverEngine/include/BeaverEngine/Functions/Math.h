#pragma once
#include <glm/glm.hpp>

namespace bv
{
	inline float length2(const glm::vec2& vec)
	{
		return vec.x * vec.x + vec.y * vec.y;
	}
}
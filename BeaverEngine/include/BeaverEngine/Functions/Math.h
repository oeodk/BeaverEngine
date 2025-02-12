#pragma once
#include <glm/glm.hpp>

namespace bv
{
	inline float length2(const glm::vec2& vec)
	{
		return vec.x * vec.x + vec.y * vec.y;
	}

	inline int sign(float v)
	{
		if (v == 0)
		{
			return 0;
		}
		if (v != abs(v))
		{
			return -1;
		}
		return 1;
	}
}
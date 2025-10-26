#pragma once
#include "glm/glm.hpp"

namespace bv
{
	struct Vertex2D
	{
		glm::vec3 position;
		glm::vec2 texture_coords;
		glm::vec4 color;
	};

	struct Vertex3D
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texture_coords;
	};
}
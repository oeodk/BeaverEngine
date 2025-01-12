#pragma once
#include "glm/glm.hpp"

namespace bv
{
	struct Vertex2D
	{
		glm::vec3 position;
		glm::vec2 texture_coords;
	};

	struct Vertex3D
	{
		glm::vec3 position;
		glm::vec3 normals;
		glm::vec2 texture_coords;
	};
}
#pragma once
#include <glm/glm.hpp>
namespace bv
{
	struct FloatRect
	{
		FloatRect() = default;
		FloatRect(const glm::vec2& i_pos, const glm::vec2& i_size) : pos(i_pos), size(i_size){}
		FloatRect(float x, float y, float width, float height) : pos(glm::vec2(x,y)), size(glm::vec2(width, height)){}
		glm::vec2 pos{};
		glm::vec2 size{1, 1};
	};
}


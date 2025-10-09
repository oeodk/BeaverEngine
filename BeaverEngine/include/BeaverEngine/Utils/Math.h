#pragma once
#include <BeaverEngine/Core/std.h>
#include <glm/glm.hpp>
namespace bv
{
	struct FloatRect
	{
		FloatRect() = default;
		FloatRect(const glm::vec2& i_pos, const glm::vec2& i_size) : pos(i_pos), size(i_size){}
		FloatRect(float x, float y, float width, float height) : pos(glm::vec2(x,y)), size(glm::vec2(width, height)){}

		FloatRect(const FloatRect&) = default;
		FloatRect(FloatRect&&) = default;
		FloatRect& operator=(const FloatRect&) = default;
		FloatRect& operator=(FloatRect&&) = default;

		glm::vec2 pos{};
		glm::vec2 size{1, 1};
	};

	class Random
	{
	public:
		static void init()
		{
			s_RandomEngine.seed(std::random_device()());
		}

		static float randomFloat()
		{
			return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
		}

	private:
		static std::mt19937 s_RandomEngine;
		static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
	};
}


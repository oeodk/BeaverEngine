#pragma once
#include "BeaverEngine/Core/std.h"

namespace bv
{
	struct Timing
	{
		std::chrono::milliseconds dt_;
		unsigned int frame_;

		float asSecond() const
		{
			return dt_.count() / 1000.f;
		}

		unsigned int asMillisecond() const
		{
			return dt_.count();
		}
	};
}
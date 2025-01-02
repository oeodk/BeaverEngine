#pragma once
#include "BeaverEngine/std.h"

namespace bv
{
	struct Timing
	{
		std::chrono::microseconds dt_;
		unsigned int frame_;
	};
}
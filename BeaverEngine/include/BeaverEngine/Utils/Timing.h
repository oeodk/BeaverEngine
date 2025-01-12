#pragma once
#include "BeaverEngine/Core/std.h"

namespace bv
{
	struct Timing
	{
		std::chrono::milliseconds dt_;
		unsigned int frame_;
	};
}
#pragma once
#include "BeaverEngine/Core/std.h"

class Timer
{
private:
	std::chrono::steady_clock::time_point start_;
	std::chrono::steady_clock::time_point end_;
public:
	void start() { start_ = std::chrono::steady_clock::now(); }
	void stop() { end_ = std::chrono::steady_clock::now(); }
	float getTime() const { return (std::chrono::duration<float>(end_ - start_)).count(); }
	float stopAndPrint(const std::string& message);
	float printTime(const std::string& message);
};

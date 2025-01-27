#include "BeaverEngine/Utils/Timer.h"

float Timer::printTime(const std::string& message)
{
	printf(std::string(message + " : " + std::to_string((std::chrono::duration<float>(end_ - start_)).count()) + "s ").c_str());
	return (std::chrono::duration<float>(end_ - start_)).count();
}

float Timer::stopAndPrint(const std::string& message)
{
	stop();
	return printTime(message);
}

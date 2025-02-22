#include "BeaverEngine/System/WindowSystem.h"
#include <GLFW/glfw3.h>

namespace bv
{
	glm::uvec2 WindowSystem::getMonitorSize(unsigned int monitor_index)
	{
		int count;
		GLFWmonitor** monitors = glfwGetMonitors(&count);
		const GLFWvidmode* videoMode = glfwGetVideoMode(monitors[monitor_index]);

		return glm::uvec2(videoMode->width, videoMode->height);
	}

	unsigned int WindowSystem::getMonitorCount()
	{
		int count;
		glfwGetMonitors(&count);
		return count;
	}
}
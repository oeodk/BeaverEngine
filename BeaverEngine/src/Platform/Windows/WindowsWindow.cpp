#include "BeaverEngine/Platform/Windows/WindowsWindow.h"
#include "BeaverEngine/Game.h"

namespace bv
{
	// Prevent multiple initialisation of glfw is multiple window are created
	static bool glfw_init = false;

	Window* Window::create(const WindowProperties& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProperties& props)
	{
		init(props);
	}

	void closeEventCallback(GLFWwindow* window)
	{
		Game::close();
	}

	void WindowsWindow::init(const WindowProperties& props)
	{
		width_ = props.width;
		height_ = props.height;
		title_ = props.title;

		if (!glfw_init)
		{
			glfwInit();
			glfw_init = true;
		}
		window_ = glfwCreateWindow(width_, height_, title_.c_str(), NULL, NULL);
		glfwMakeContextCurrent(window_);
		setVSync(true);

		glfwSetWindowCloseCallback(window_, closeEventCallback);
	}

	WindowsWindow::~WindowsWindow()
	{
		shutdown();
	}

	void WindowsWindow::shutdown()
	{
		glfwDestroyWindow(window_);
	}

	void WindowsWindow::onUpdate()
	{
		/* Swap front and back buffers */
		glfwSwapBuffers(window_);

		/* Poll for and process events */
		glfwPollEvents();
	}

	void WindowsWindow::setVSync(bool enabled)
	{
		if (enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}
		v_sync_ = enabled;
	}
}
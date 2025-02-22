#include "BeaverEngine/Platform/Desktop/DesktopWindow.h"
#include "BeaverEngine/System/WindowSystem.h"
#include "BeaverEngine/Core/Scene.h"
#include "BeaverEngine/Core/Entity.h"
#include "BeaverEngine/Utils/Vertex.h"
#include "BeaverEngine/System/InputSystem.h"

#include "BeaverEngine/System/WindowSystem.h"
namespace bv
{
	// Prevent multiple initialisation of glfw is multiple window are created
	static bool glfw_init = false;
	static bool glad_init = false;

	std::shared_ptr<Window> Window::create(const WindowProperties& props)
	{
		return std::make_shared<DesktopWindow>(props);
	}

	DesktopWindow::DesktopWindow(const WindowProperties& props)
	{
		init(props);
	}

	void closeEventCallback(GLFWwindow* window)
	{
		DesktopWindow::Properties* properties = static_cast<DesktopWindow::Properties*>(glfwGetWindowUserPointer(window));

		WindowSystem::getInstance().closeWindow(properties->self->getName());
	}

	void resizeEventCallback(GLFWwindow* window, int width, int height)
	{
		DesktopWindow::Properties* properties = static_cast<DesktopWindow::Properties*>(glfwGetWindowUserPointer(window));
		properties->width = width;
		properties->height = height;
	}

	void keyEventCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if(action == GLFW_RELEASE || action == GLFW_PRESS)
		{
			InputSystem::getInstance().setKeyState(Key::Key(key), action);
		}
	}

	void mouseButtonEventCallback(GLFWwindow* window, int button, int action, int mods)
	{
		if (action == GLFW_RELEASE || action == GLFW_PRESS)
		{
			InputSystem::getInstance().setMouseButtonState(Mouse::Button(button), action != GLFW_RELEASE);
		}
	}

	void scrollEventCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		InputSystem::getInstance().setMouseScrollValue(yoffset);
	}

	void DesktopWindow::init(const WindowProperties& props)
	{
		name_ = props.name;

		properties.width = props.width;
		properties.height = props.height;
		properties.title = props.title;
		properties.background_color = props.color;
		properties.self = this;
		if (!glfw_init)
		{
			glfwInit();
			glfw_init = true;

			const auto& monitor_size_ = WindowSystem::getInstance().getMonitorSize();
			monitor_center_ = { monitor_size_.x / 2.f, monitor_size_.y / 2.f };
			
		}

		if (props.resizable)
		{
			glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		}
		else
		{
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		}
		if (props.decorate)
		{
			glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
		}
		else
		{
			glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
		}
		
		GLFWwindow* shared_window = nullptr;
		if (props.shared != nullptr)
		{
			shared_window = dynamic_cast<DesktopWindow*>(props.shared)->window_;

		}
		window_ = glfwCreateWindow(properties.width, properties.height, properties.title.c_str(), NULL, shared_window);
		glfwMakeContextCurrent(window_);
		setVSync(true);

		if (!glad_init)
		{
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				printf("GLAD loading error");
				return;
			}
			glad_init = true;
		}

		setPosition(props.position.x, props.position.y);

		glfwSetWindowUserPointer(window_, &properties);
		glfwSetWindowCloseCallback(window_, closeEventCallback);
		glfwSetWindowSizeCallback(window_, resizeEventCallback);
		glfwSetKeyCallback(window_, keyEventCallback);
		glfwSetMouseButtonCallback(window_, mouseButtonEventCallback);
		glfwSetScrollCallback(window_, scrollEventCallback);

		glCreateVertexArrays(1, &vao_2d_);
		glBindVertexArray(vao_2d_);

		glEnableVertexAttribArray(0);
		glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex2D, position));
		glVertexAttribBinding(0, 0); // Link attribute 0 to binding index 0

		// Attribute 1: Texture Coordinates (vec2)
		glEnableVertexAttribArray(1);
		glVertexAttribFormat(1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex2D, texture_coords));
		glVertexAttribBinding(1, 0); // Link attribute 1 to binding index 0
		
		// Attribute 2: Color (vec4)
		glEnableVertexAttribArray(2);
		glVertexAttribFormat(2, 4, GL_FLOAT, GL_FALSE, offsetof(Vertex2D, color));
		glVertexAttribBinding(2, 0); // Link attribute 2 to binding index 0

		// Unbind the VAO (optional)
		glBindVertexArray(0);
	}

	DesktopWindow::~DesktopWindow()
	{
		if(open)
			shutdown();
	}

	void DesktopWindow::shutdown()
	{
		open = false;
		glfwDestroyWindow(window_);
	}

	void DesktopWindow::setPosition(int x, int y)
	{
		position_.x = x;
		position_.y = y;
		glfwSetWindowPos(window_, -(properties.width / 2.f) + monitor_center_.x + position_.x, -(properties.height / 2.f) + monitor_center_.y - position_.y);
	}

	void DesktopWindow::move(int dx, int dy)
	{
		setPosition(position_.x + dx, position_.y + dy);
	}

	void DesktopWindow::onUpdate()
	{
		/* Swap front and back buffers */
		glfwSwapBuffers(window_);

		/* Poll for and process events */
		glfwPollEvents();
		cleared_ = false;
	}

	void DesktopWindow::clear()
	{
		if(!cleared_)
		{
			glClearColor(properties.background_color.r, properties.background_color.g, properties.background_color.b, properties.background_color.a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			cleared_ = true;
		}
	}

	void DesktopWindow::beginRender2d() const
	{
		glBindVertexArray(vao_2d_);
	}

	void DesktopWindow::setBackgroundColor(const glm::vec4& color)
	{
		properties.background_color = color;
	}

	const glm::vec4& DesktopWindow::getBackgroundColor() const
	{
		return properties.background_color;
	}

	void DesktopWindow::setVSync(bool enabled)
	{
		if (enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}
		properties.v_sync = enabled;
	}
}
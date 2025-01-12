#include "BeaverEngine/Platform/Desktop/DesktopWindow.h"
#include "BeaverEngine/Component/WindowManagerComponent.h"
#include "BeaverEngine/Core/Scene.h"
#include "BeaverEngine/Core/Entity.h"
#include "BeaverEngine/Utils/Vertex.h"
#include "BeaverEngine/Component/InputManagerComponent.h"

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

		ManagerComponent::getManager<WindowManagerComponent>()->closeWindow(properties->self->getName());
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
			ManagerComponent::getManager<InputManagerComponent>()->setKeyState(Key::Key(key), action);
		}
	}

	void mouseButtonEventCallback(GLFWwindow* window, int button, int action, int mods)
	{
		if (action == GLFW_RELEASE || action == GLFW_PRESS)
		{
			ManagerComponent::getManager<InputManagerComponent>()->setMouseButtonState(Mouse::Button(button), action != GLFW_RELEASE);
		}
	}

	void scrollEventCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		ManagerComponent::getManager<InputManagerComponent>()->setMouseScrollValue(yoffset);
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
		glfwSetWindowUserPointer(window_, &properties);
		glfwSetWindowCloseCallback(window_, closeEventCallback);
		glfwSetWindowSizeCallback(window_, resizeEventCallback);
		glfwSetKeyCallback(window_, keyEventCallback);
		glfwSetMouseButtonCallback(window_, mouseButtonEventCallback);
		glfwSetScrollCallback(window_, scrollEventCallback);
		//glGenVertexArrays(1, &vao_2d_);

		glCreateVertexArrays(1, &vao_2d_);
		glBindVertexArray(vao_2d_);

		glEnableVertexAttribArray(0);
		glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex2D, position));
		glVertexAttribBinding(0, 0); // Link attribute 0 to binding index 0

		// Attribute 1: Texture Coordinates (vec2)
		glEnableVertexAttribArray(1);
		glVertexAttribFormat(1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex2D, texture_coords));
		glVertexAttribBinding(1, 0); // Link attribute 1 to binding index 0

		// Unbind the VAO (optional)
		glBindVertexArray(0);

		//glEnableVertexAttribArray(0);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (const void*)offsetof(Vertex2D, position));
		//
		//glEnableVertexAttribArray(1);
		//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (const void*)offsetof(Vertex2D, texture_coords));
		//
		//glBindVertexArray(0);
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
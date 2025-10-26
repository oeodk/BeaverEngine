#include "BeaverEngine/Platform/PlatformMacros.h"
#ifdef PLATFORM_DESKTOP

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

	void focusChangeEventCallback(GLFWwindow* window, int focused)
	{
		DesktopWindow::Properties* properties = static_cast<DesktopWindow::Properties*>(glfwGetWindowUserPointer(window));
		properties->is_focused = focused;
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
		properties.is_focused = true;
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
		if (properties.background_color.a < 1)
		{
			glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
		}
		GLFWwindow* shared_window = nullptr;
		if (props.shared != nullptr)
		{
			shared_window = dynamic_cast<DesktopWindow*>(props.shared)->window_;

		}
		window_ = glfwCreateWindow(properties.width, properties.height, properties.title.c_str(), NULL, shared_window);
		glfwMakeContextCurrent(window_);
		setVSync(props.vsync);

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
		glfwSetWindowFocusCallback(window_, focusChangeEventCallback);

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

	void DesktopWindow::updateGamepadInputs() const
	{
		auto& input_system = InputSystem::getInstance();

		for (int joystick_id = Joystick::JOYSTICK_1; joystick_id < Joystick::JOYSTICK_16; joystick_id++)
		{
			GLFWgamepadstate state;

			if (glfwGetGamepadState(joystick_id, &state))
			{
				for (int i = 0; i < 15 ; i++)
				{
					input_system.setControllerButtonState(static_cast<Joystick::Joystick>(joystick_id), static_cast<Gamepad::Gamepad>(i), state.buttons[i] != GLFW_RELEASE);
				}
				for (int i = 0; i < 6; i++)
				{
					Axis::Axis axis = static_cast<Axis::Axis>(i);
					int coefficient = 1;
					if (axis == Axis::LEFT_Y || axis == Axis::RIGHT_Y)
					{
						coefficient = -1;
					}
					input_system.setJoystickAxisValue(static_cast<Joystick::Joystick>(joystick_id), axis, coefficient * state.axes[i]);
				}
			}
			else
			{
				break;
			}
		}
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

		updateGamepadInputs();

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

#endif
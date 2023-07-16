#include "Window.h"

#include"Systems/RendererSystem.h"
#include"Core/Logger.h"

Window::Window(unsigned int width, unsigned int height, std::string title) :
	m_size(width,height), m_title(title), m_window(nullptr)
{

}

Window::~Window()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Window::init()
{
	Logger::Info("Creating a window named {0} size {1}x{2}", m_title, m_size.x, m_size.y);

	if (!glfwInit())
	{
		Logger::Critical("ERROR::GLFW::Failed to load glfw!");
		return;
	}

	m_window = glfwCreateWindow(m_size.x, m_size.y, m_title.c_str(), nullptr, nullptr);
	if (!m_window)
	{
		Logger::Critical("ERROR::GLFW::Failed to create a window!");
		return;
	}

	glfwMakeContextCurrent(m_window);

	glfwSetErrorCallback([](int error_code, const char* description) 
		{
			Logger::Critical("ERROR::GLFW::{0}",description);
		});
	glfwSetWindowUserPointer(m_window, this);
	glfwSetCursorPosCallback(m_window, [](GLFWwindow* glfwWindow, double x, double y)
		{
			Window& window = *static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
			EventMouseMoved event(x, y);
			window.getEventCallback()(event);
		});
	glfwSetMouseButtonCallback(m_window, [](GLFWwindow* glfwWindow, int button, int action, int mods)
		{
			Window& window = *static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));

			double x, y;
			glfwGetCursorPos(glfwWindow, &x, &y);

			switch (action)
			{
			case GLFW_PRESS:
			{
				EventMouseButtonPressed event(static_cast<MouseCode>(button), x, y);
				window.getEventCallback()(event);
				break;
			}
			case GLFW_RELEASE:
			{
				EventMouseButtonReleased event(static_cast<MouseCode>(button), x, y);
				window.getEventCallback()(event);
				break;
			}
			}
		});
	glfwSetKeyCallback(m_window, [](GLFWwindow* glfwWindow, int key, int scancode, int action, int mods)
		{
			Window& window = *static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
			switch (action)
			{
			case GLFW_PRESS:
			{
				EventKeyPressed event(static_cast<KeyCode>(key));
				window.getEventCallback()(event);
				break;
			}
			case GLFW_REPEAT:
			{
				EventKeyPressed event(static_cast<KeyCode>(key),true);
				window.getEventCallback()(event);
				break;
			}
			case GLFW_RELEASE:
			{
				EventKeyReleased event(static_cast<KeyCode>(key));
				window.getEventCallback()(event);
				break;
			}
			}
		});
	glfwSetWindowCloseCallback(m_window, [](GLFWwindow* glfwWindow)
		{
			Window& window = *static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
			EventWindowClose event;
			window.getEventCallback()(event);
		});
	glfwSetWindowSizeCallback(m_window, [](GLFWwindow* glfwWindow, int width, int height)
		{
			Window& window = *static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
			
			EventWindowResize event(width, height);
			window.getEventCallback()(event);
		});
	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* glfwWindow, int width, int height)
		{
			Window& window = *static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
			System::GetInstance<RendererSystem>()->updateViewport({ width, height });
		});
}

void Window::update()
{
	glfwPollEvents();
	glfwSwapBuffers(m_window);
}

void Window::setEventCallback(std::function<void(Event&)> callback)
{
	m_eventCallback = callback;
}

std::function<void(Event&)> Window::getEventCallback()
{
	return m_eventCallback;
}

GLFWwindow* Window::getWindow()
{
	return m_window;
}

void Window::setWidth(unsigned int width)
{
	m_size.x = width;
}

void Window::setHeight(unsigned int height)
{
	m_size.y = height;
}

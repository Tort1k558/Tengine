#pragma once

#include<string>
#include<functional>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"Event.h"
class Window
{
public:
	Window(unsigned int width, unsigned int height, std::string title);
	~Window();
	Window(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator=(const Window&) = delete;
	Window& operator=(Window&&) = delete;

	void init();
	void update();
	void setEventCallback(std::function<void(Event&)> callback);
	std::function<void(Event&)> getEventCallback();
	unsigned int getWidth();
	unsigned int getHeight();
	GLFWwindow* getWindow();
	void setWidth(unsigned int width);
	void setHeight(unsigned int height);
private:
	GLFWwindow* m_window;
	unsigned int m_width;
	unsigned int m_height;
	std::string m_title;
	std::function<void(Event&)> m_eventCallback;
};
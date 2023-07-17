#pragma once

#include<string>
#include<functional>
#include<GLFW/glfw3.h>
#include"Event.h"
#include"Core/Math.h"
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
	void setWidth(unsigned int width);
	void setHeight(unsigned int height);
	void setCursorPos(UVec2 pos);
	std::function<void(Event&)> getEventCallback();
	GLFWwindow* getWindow();
	UVec2 getSize() { return m_size; }
	unsigned int getWidth() { return m_size.x; }
	unsigned int getHeight() { return m_size.y; }
private:
	GLFWwindow* m_window;
	UVec2 m_size;
	std::string m_title;
	std::function<void(Event&)> m_eventCallback;
};
#pragma once

#include<string>
#include<memory>
#include"Event.h"

class Window;

class Application
{
public:
	Application(unsigned int width, unsigned int height, std::string title);
	~Application();
	Application(const Application&) = delete;
	Application(Application&&) = delete;
	Application& operator=(const Application&) = delete;
	Application& operator=(Application&&) = delete;

	void init();
	void run();
private:
	std::shared_ptr<Window> m_window;
	EventDispatcher m_eventDispatcher;
	bool m_closeWindow;
};
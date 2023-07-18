#pragma once

#include<string>
#include<memory>
#include"Event.h"

class Window;

class Application
{
public:
	Application(unsigned int width, unsigned int height, std::string title);
	virtual ~Application();
	Application(const Application&) = delete;
	Application(Application&&) = delete;
	Application& operator=(const Application&) = delete;
	Application& operator=(Application&&) = delete;

	virtual void update() = 0;
	virtual void create() = 0;

	void init();
	void run();
	void lockFps();
	void unlockFps();
	void setMaxFps(size_t fps);
	size_t getMaxFps() { return m_maxFps; }
	void close();
private:
	std::shared_ptr<Window> m_window;
	EventDispatcher m_eventDispatcher;
	bool m_closeWindow;
	bool m_lockFps;
	size_t m_maxFps;
};
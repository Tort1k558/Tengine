#pragma once

#include<string>
#include<memory>
#include"Event.h"
#include"Core/Window.h"
#include"Core/Core.h"


namespace Tengine
{
	class Window;

	class TENGINE Application
	{
	public:
		Application(unsigned int width, unsigned int height, std::string title);
		virtual ~Application() = default;

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
		void close();
		
		size_t getMaxFps() const;
		std::shared_ptr<Window> getWindow() const;
	private:
		std::shared_ptr<Window> m_window;
		EventDispatcher m_eventDispatcher;
		bool m_isRunning = true;
		bool m_lockFps = false;
		size_t m_maxFps = 0;
	};
}
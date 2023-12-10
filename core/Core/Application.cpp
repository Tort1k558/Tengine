#include "Application.h"

#include<thread>

#include"Components/Transform.h"
#include"Components/Camera.h"
#include"Core/Timer.h"
#include"Core/Logger.h"
#include"Core/Input.h"
#include"Scene/SystemManager.h"
#include"Systems/RendererSystem.h"
#include"Scene/SceneManager.h"

namespace Tengine
{
    Application::Application(unsigned int width, unsigned int height, std::string title)
    {
        m_window = std::make_shared<Window>(width, height, title);
    }

    void Application::init()
    {
        m_window->init();

        RendererSystem::GetInstance()->setRendererType(RendererType::OpenGL);
        RendererSystem::GetInstance()->setTextureFilter(TextureFilter::Bilinear);
        RendererSystem::GetInstance()->updateViewport(m_window->getSize());
        RendererSystem::GetInstance()->init();

        m_eventDispatcher = EventDispatcher();
        m_eventDispatcher.addEvent<EventMouseMoved>([](EventMouseMoved& event)
            {
                Input::SetMousePosition({ event.x, event.y });
            });
        m_eventDispatcher.addEvent<EventMouseButtonPressed>([](EventMouseButtonPressed& event)
            {
                Input::PressMouseButton(event.code);
            });
        m_eventDispatcher.addEvent<EventMouseButtonReleased>([](EventMouseButtonReleased& event)
            {
                Input::ReleaseMouseButton(event.code);
            });
        m_eventDispatcher.addEvent<EventKeyPressed>([](EventKeyPressed& event)
            {
                Input::PressKey(event.code);
            });
        m_eventDispatcher.addEvent<EventKeyReleased>([](EventKeyReleased& event)
            {
                Input::ReleaseKey(event.code);
            });
        m_eventDispatcher.addEvent<EventWindowClose>([&](EventWindowClose& event)
            {
                Logger::Info("EVENT::The Window closed");
                m_isRunning = false;
            });
        m_window->setEventCallback([&](Event& event)
            {
                m_eventDispatcher.proccess(event);
            });

        create();
        SystemManager::InitSystems();
    }

    void Application::run()
    {

        double maxDelta = 1.0 / static_cast<double>(m_maxFps);
        std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<double>> nextFrame = Timer::GetNowPoint();

        while (m_isRunning)
        {
            if (Timer::GetDeltaTime() < maxDelta && m_lockFps)
            {
                std::this_thread::sleep_until(nextFrame);
                Timer::SetDeltaTime(maxDelta);
            }

            tick();

            if (m_lockFps)
            {
                nextFrame += std::chrono::duration<double>(maxDelta);
            }
        }   
    }

    void Application::lockFps()
    {
        m_lockFps = true;
    }

    void Application::unlockFps()
    {
        m_lockFps = false;
    }

    void Application::setMaxFps(size_t fps)
    {
        m_maxFps = fps;
    }

    void Application::destroy()
    {
        m_isRunning = false;

        close();
        SystemManager::DestroySystems();
        RendererSystem::GetInstance()->destroy();
    }

    void Application::tick()
    {
        Timer::Start();

        Input::Update();
        m_window->update();
        RendererSystem::GetInstance()->update();
        SystemManager::UpdateSystems();
        update();

        Timer::End();
    }
    size_t Application::getMaxFps() const
    {
        return m_maxFps;
    }
    std::shared_ptr<Window> Application::getWindow() const
    {
        return m_window;
    }
}
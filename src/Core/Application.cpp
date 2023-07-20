#include "Application.h"

#include<thread>

#include"Components/Transform.h"
#include"Components/Camera.h"
#include"Components/Controller.h"
#include"Core/Timer.h"
#include"Core/Logger.h"
#include"Core/Input.h"
#include"ECS/SystemManager.h"
#include"Systems/RendererSystem.h"
#include"Systems/UISystem.h"
#include"Systems/ControllerSystem.h"
#include"Scene/SceneManager.h"

Application::Application(unsigned int width, unsigned int height, std::string title)
{
	m_window = std::make_shared<Window>(width, height, title);
}

void Application::init()
{
	m_window->init();

    System::GetInstance<RendererSystem>()->setRendererType(RendererType::OpenGL);
    System::GetInstance<RendererSystem>()->setTextureFilter(TextureFilter::Anisotropic16);
    SystemManager::AddSystem<RendererSystem>();
    System::GetInstance<UISystem>()->setWindow(m_window);
    SystemManager::AddSystem<UISystem>();
    SystemManager::AddSystem<ControllerSystem>();
    SystemManager::InitSystems();

    System::GetInstance<RendererSystem>()->updateViewport(m_window->getSize());

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
}

void Application::run()
{

    double maxDelta = 1.0 / static_cast<double>(m_maxFps);
    std::chrono::time_point<std::chrono::steady_clock,std::chrono::duration<double>> nextFrame = Timer::GetNowPoint();

    while (m_isRunning)
    {
        if (Timer::GetDeltaTime() < maxDelta && m_lockFps)
        {
            std::this_thread::sleep_until(nextFrame);
            Timer::SetDeltaTime(maxDelta);
        }

        Timer::Start();

        Input::Update();
        m_window->update();
        SystemManager::UpdateSystems();

        update();
        Timer::End();

        if (m_lockFps)
        {
            nextFrame += std::chrono::duration<double>(maxDelta);
        }
    }
    SystemManager::DestroySystems();
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

void Application::close()
{
    m_isRunning = false;
}


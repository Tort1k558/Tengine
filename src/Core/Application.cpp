#include "Application.h"

#include<thread>

#include"Components/Transform.h"
#include"Components/Camera.h"
#include"Core/Timer.h"
#include"Core/Logger.h"
#include"Core/Input.h"
#include"ECS/SystemManager.h"
#include"Systems/RendererSystem.h"
#include"Systems/UISystem.h"
#include"Scene/SceneManager.h"

Application::Application(unsigned int width, unsigned int height, std::string title) :
    m_closeWindow(false)
{
	m_window = std::make_shared<Window>(width, height, title);
}

Application::~Application()
{
    
}
std::shared_ptr<Transform> transform2;
void Application::init()
{
	m_window->init();

    System::GetInstance<RendererSystem>()->setRendererType(RendererType::OpenGL);
    SystemManager::AddSystem<RendererSystem>();
    System::GetInstance<UISystem>()->setWindow(m_window);
    SystemManager::AddSystem<UISystem>();

    SystemManager::InitSystems();

    std::shared_ptr<Scene> scene = Scene::Create();
    SceneManager::SetCurrentScene(scene);

    std::shared_ptr<Object> object = Object::Create();
    std::shared_ptr<Transform> transform = object->getComponent<Transform>();
    transform->setScale({ 2.0f,1.0f,1.0f });
    transform->setPosition({ 0.5f,0.0f,0.0f });

    std::shared_ptr<Object> object2 = Object::Create();
    object2->addComponent<Camera>(Component::Create<Camera>(ProjectionType::Perspective));
    transform2 = object2->getComponent<Transform>();
    transform2->setRotation({ 0.0f,0.0f, 0.0f });
    transform2->setPosition({ 0.0f,0.0f,-2.0f });

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
            Logger::Debug("EVENT::The Window closed");
            m_closeWindow = true;
        });
    m_eventDispatcher.addEvent<EventWindowResize>([](EventWindowResize& event)
        {
            Logger::Debug("EVENT::The Window resized to {0}x{1}", event.x, event.y);
        });
    m_window->setEventCallback([&](Event& event)
        {
            m_eventDispatcher.proccess(event);
        });
}

void Application::run()
{
    double speed = 50.0;
    double delta = 0.0;
    unsigned int maxFps = 144;
    double maxDelta = 1.0 / static_cast<double>(maxFps);
    while (!m_closeWindow)
    {
        Timer::Start();
        if (delta < maxDelta)
        {
            delta += Timer::GetDeltaTime();
        }
        else
        {
            Timer::SetDeltaTime(delta);
            SystemManager::UpdateSystems();
            if (Input::IsKeyPressed(KeyCode::D))
            {
                transform2->setPosition(transform2->getPosition() + Vec3(-speed * Timer::GetDeltaTime(), 0.0f, 0.0f));
            }
            if (Input::IsKeyPressed(KeyCode::A))
            {
                transform2->setPosition(transform2->getPosition() + Vec3(speed * Timer::GetDeltaTime(), 0.0f, 0.0f));
            }
            if (Input::IsKeyPressed(KeyCode::W))
            {
                transform2->setPosition(transform2->getPosition() + Vec3(0.0f, 0.0f, speed * Timer::GetDeltaTime()));
            }
            if (Input::IsKeyPressed(KeyCode::S))
            {
                transform2->setPosition(transform2->getPosition() + Vec3(0.0f, 0.0f, -speed * Timer::GetDeltaTime()));
            }
            Logger::Debug("{0:.9f}", Timer::GetDeltaTime());
            m_window->update();
            delta = 0.0f;
        }
        Timer::End();
    }
    SystemManager::DestroySystems();
}


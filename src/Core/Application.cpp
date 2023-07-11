#include "Application.h"

#include "Components/Transform.h"
#include "Core/Timer.h"
#include "Core/Logger.h"
#include "ECS/Object.h"
#include "ECS/SystemManager.h"
#include "Systems/RendererSystem.h"
#include "Systems/UISystem.h"
#include"Core/UUID.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

Application::Application(unsigned int width, unsigned int height, std::string title) :
    m_closeWindow(false)
{
	m_window = std::make_shared<Window>(width, height, title);
}

Application::~Application()
{
    
}

void Application::init()
{
	m_window->init();

    System::GetInstance<RendererSystem>()->setRendererType(RendererType::OpenGL);
    SystemManager::AddSystem<RendererSystem>();
    System::GetInstance<UISystem>()->setWindow(m_window);
    SystemManager::AddSystem<UISystem>();

    SystemManager::InitSystems();

    m_eventDispatcher = EventDispatcher();
    m_eventDispatcher.addEvent<EventMouseMoved>([](EventMouseMoved& event)
        {
            //Logger::Debug("EVENT::The Mouse Moved to {0}x{1}", event.xPos, event.yPos);
        });
    m_eventDispatcher.addEvent<EventMouseButtonPressed>([](EventMouseButtonPressed& event)
        {
            Logger::Debug("EVENT::The mouse button is pressed with the code {0} on the cordinates {1}x{2}", static_cast<int>(event.code), event.x, event.y);
        });
    m_eventDispatcher.addEvent<EventMouseButtonReleased>([](EventMouseButtonReleased& event)
        {
            Logger::Debug("EVENT::The mouse button is released with the code {0} on the cordinates {1}x{2}", static_cast<int>(event.code), event.x, event.y);
        });
    m_eventDispatcher.addEvent<EventKeyPressed>([&](EventKeyPressed& event)
        {
            Logger::Debug("EVENT::The mouse button is pressed with the code {0}", static_cast<int>(event.code));
        });
    m_eventDispatcher.addEvent<EventKeyReleased>([&](EventKeyReleased& event)
        {
            Logger::Debug("EVENT::The mouse button is released with the code {0}", static_cast<int>(event.code));;
        });
    m_eventDispatcher.addEvent<EventWindowClose>([&](EventWindowClose& event)
        {
            Logger::Debug("EVENT::The Window closed");
            m_closeWindow = true;
        });
    m_eventDispatcher.addEvent<EventWindowResize>([&](EventWindowResize& event)
        {
            Logger::Debug("EVENT::The Window resized to {0}x{1}", event.x, event.y);
        });
    m_window->setEventCallback([&](Event& event)
        {
            m_eventDispatcher.proccess(event);
        });
    
    std::shared_ptr<Scene> scene = Scene::Create();
    SceneManager::SetCurrentScene(scene);

    std::shared_ptr<Object> object = Object::Create();
    std::shared_ptr<Object> object2 = Object::Create();
    std::shared_ptr<Transform> transform = Component::Create<Transform>();
    transform->setRotation({ 0.0f,0.0f,90.0f });
    transform->setScale({ 2.0f,1.0f,1.0f });
    transform->setPosition({ 0.5f,0.0f,0.0f });
    object->addComponent(transform);

}

void Application::run()
{
    while (!m_closeWindow)
    {
        Timer::Start();
        m_window->update();
        SystemManager::UpdateSystems();
    }
    SystemManager::DestroySystems();
}


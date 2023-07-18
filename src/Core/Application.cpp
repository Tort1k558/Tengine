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
std::shared_ptr<Camera> camera;
void Application::init()
{
	m_window->init();

    System::GetInstance<RendererSystem>()->setRendererType(RendererType::OpenGL);
    System::GetInstance<RendererSystem>()->setTextureFilter(TextureFilter::Anisotropic16);
    SystemManager::AddSystem<RendererSystem>();
    System::GetInstance<UISystem>()->setWindow(m_window);
    SystemManager::AddSystem<UISystem>();

    SystemManager::InitSystems();

    System::GetInstance<RendererSystem>()->updateViewport(m_window->getSize());

    std::shared_ptr<Scene> scene = Scene::Create();
    SceneManager::SetCurrentScene(scene);

    std::shared_ptr<Object> object = Object::Create();
    std::shared_ptr<Transform> transform = object->getComponent<Transform>();
    transform->setScale({ 20.0f,20.0f,1.0f });
    transform->setRotationY(90.0f);

    std::shared_ptr<Object> object2 = Object::Create();
    camera = Component::Create<Camera>(ProjectionType::Perspective);
    object2->addComponent<Camera>(camera);
    transform2 = object2->getComponent<Transform>();
    transform2->setPosition({ 0.0f,0.0f,2.0f });

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
    double speed = 7.0;
    double cameraSensitivity= 0.314;

    double maxDelta = 1.0 / static_cast<double>(m_maxFps);
    std::chrono::time_point<std::chrono::steady_clock,std::chrono::duration<double>> nextFrame = Timer::GetNowPoint();

    Vec2 prevMousePos = Input::GetMousePosition();
    Vec2 mousePos = Input::GetMousePosition();
    Vec2 deltaMouse(0.0,0.0);
    while (!m_closeWindow)
    {
        if (Timer::GetDeltaTime() < maxDelta && m_lockFps)
        {
            std::this_thread::sleep_until(nextFrame);
            Timer::SetDeltaTime(maxDelta);
        }

        Timer::Start();
        prevMousePos = Input::GetMousePosition();

        SystemManager::UpdateSystems();
        m_window->update();
        if (Input::IsKeyPressed(KeyCode::ESCAPE))
        {
            m_closeWindow = true;
        }
        if (Input::IsKeyPressed(KeyCode::D))
        {
            transform2->setPosition(transform2->getPosition() + Normalize(Cross(camera->getDirection(), camera->getUp())) * Vec3(speed * Timer::GetDeltaTime()));
        }
        if (Input::IsKeyPressed(KeyCode::A))
        {
            transform2->setPosition(transform2->getPosition() + Normalize(Cross(camera->getDirection(), camera->getUp())) * Vec3(-speed * Timer::GetDeltaTime()));
        }
        if (Input::IsKeyPressed(KeyCode::W))
        {
            transform2->setPosition(transform2->getPosition() + camera->getDirection() * Vec3(speed * Timer::GetDeltaTime()));
        }
        if (Input::IsKeyPressed(KeyCode::S))
        {
            transform2->setPosition(transform2->getPosition() + camera->getDirection() * Vec3(-speed * Timer::GetDeltaTime()));
        }
        if (Input::IsKeyPressed(KeyCode::LEFT_SHIFT))
        {
            transform2->setPosition(transform2->getPosition() + Vec3(0.0f, -speed * Timer::GetDeltaTime(), 0.0f));
        }
        if (Input::IsKeyPressed(KeyCode::SPACE))
        {
            transform2->setPosition(transform2->getPosition() + Vec3(0.0f, speed * Timer::GetDeltaTime(), 0.0f));
        }
        if (deltaMouse.x != 0)
        {
            transform2->setRotationZ(transform2->getRotation().z + deltaMouse.x * cameraSensitivity);
        }
        if (deltaMouse.y != 0)
        {
            if (transform2->getRotation().y + deltaMouse.y * cameraSensitivity < 89.0f && transform2->getRotation().y + deltaMouse.y * cameraSensitivity > -89.0f)
            {
                transform2->setRotationY(transform2->getRotation().y + deltaMouse.y * cameraSensitivity);
            }
        }

        deltaMouse = prevMousePos - Input::GetMousePosition();
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


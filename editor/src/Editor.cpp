#include "Editor.h"

#include"UISystem.h"
#include"ECS/SystemManager.h"
#include"ECS/Components/Transform.h"
#include"ECS/Components/Camera.h"
#include"ECS/Components/Controller.h"
#include"ECS/Components/Mesh.h"
#include"Core/Timer.h"
#include"Core/Logger.h"
#include"Core/Input.h"
#include"Scene/SceneManager.h"
#include"Utils/Primitives.h"
#include"Core/AssetManager.h"

Editor::Editor(unsigned int width, unsigned int height, const std::string& title) :
Application(width, height, title)
{

}

void Editor::create()
{
    UISystem::GetInstance<UISystem>()->setWindow(getWindow());
    SystemManager::AddSystem<UISystem>();
    UISystem::GetInstance<UISystem>()->init();
    std::shared_ptr<Scene> scene = Scene::Create();
    SceneManager::SetCurrentScene(scene);
    
    std::shared_ptr<Object> object = Object::Create();
    std::shared_ptr<Transform> transform = object->getComponent<Transform>();
    transform->setScale({ 10.0f,10.0f,1.0f });
    transform->setPositionY(-1.0f);
    transform->setRotationX(-90.0f);
    object->addComponent<Mesh>(Primitives::CreateSphere(50,50));

    std::shared_ptr<Object> object2 = Object::Create();
    std::shared_ptr<Camera> camera = Component::Create<Camera>(ProjectionType::Perspective);
    object2->addComponent<Camera>(camera);
    object2->setName("Camera");
    std::shared_ptr<Transform> transform2 = object2->getComponent<Transform>();
    transform2->setPosition({ 0.0f,0.0f,2.0f });

    double speed = 7.0;
    float cameraSensitivity = 0.314f;

    std::shared_ptr<Controller> controller = Component::Create<Controller>();
    controller->addKeyCallback(KeyCode::W, [speed](std::shared_ptr<Object> object)
        {
            std::shared_ptr<Transform> transform = object->getComponent<Transform>();
            std::shared_ptr<Camera> camera = object->getComponent<Camera>();
            transform->setPosition(transform->getPosition() + camera->getDirection() * Vec3(static_cast<float>(speed * Timer::GetDeltaTime())));
        });
    controller->addKeyCallback(KeyCode::S, [speed](std::shared_ptr<Object> object)
        {
            std::shared_ptr<Transform> transform = object->getComponent<Transform>();
            std::shared_ptr<Camera> camera = object->getComponent<Camera>();
            transform->setPosition(transform->getPosition() + camera->getDirection() * Vec3(static_cast<float>(-speed * Timer::GetDeltaTime())));
        });
    controller->addKeyCallback(KeyCode::A, [speed](std::shared_ptr<Object> object)
        {
            std::shared_ptr<Transform> transform = object->getComponent<Transform>();
            std::shared_ptr<Camera> camera = object->getComponent<Camera>();
            transform->setPosition(transform->getPosition()
                + Normalize(Cross(camera->getDirection(), camera->getUp()))
                * Vec3(static_cast<float>(-speed * Timer::GetDeltaTime())));
        });
    controller->addKeyCallback(KeyCode::D, [speed](std::shared_ptr<Object> object)
        {
            std::shared_ptr<Transform> transform = object->getComponent<Transform>();
            std::shared_ptr<Camera> camera = object->getComponent<Camera>();
            transform->setPosition(transform->getPosition()
                + Normalize(Cross(camera->getDirection(), camera->getUp()))
                * Vec3(static_cast<float>(speed * Timer::GetDeltaTime())));
        });
    controller->addKeyCallback(KeyCode::LEFT_SHIFT, [speed](std::shared_ptr<Object> object)
        {
            std::shared_ptr<Transform> transform = object->getComponent<Transform>();
            std::shared_ptr<Camera> camera = object->getComponent<Camera>();
            transform->setPosition(transform->getPosition() + Vec3(0.0f, static_cast<float>(-speed * Timer::GetDeltaTime()), 0.0f));
        });
    controller->addKeyCallback(KeyCode::SPACE, [speed](std::shared_ptr<Object> object)
        {
            std::shared_ptr<Transform> transform = object->getComponent<Transform>();
            std::shared_ptr<Camera> camera = object->getComponent<Camera>();
            transform->setPosition(transform->getPosition() + Vec3(0.0f, static_cast<float>(speed * Timer::GetDeltaTime()), 0.0f));
        });
    controller->setMouseMoveCallback([cameraSensitivity](std::shared_ptr<Object> object)
        {
            std::shared_ptr<Transform> transform = object->getComponent<Transform>();
            if (Input::IsMouseButtonPressed(MouseButton::BUTTON_2))
            {
                static Vec2 deltaMouse(0.0f, 0.0f);
                if (deltaMouse.x != 0)
                {
                    transform->setRotationY(transform->getRotation().y + deltaMouse.x * cameraSensitivity);
                }
                if (deltaMouse.y != 0)
                {
                    if (transform->getRotation().x + deltaMouse.y * cameraSensitivity < 89.0f && transform->getRotation().x + deltaMouse.y * cameraSensitivity > -89.0f)
                    {
                        transform->setRotationX(transform->getRotation().x + deltaMouse.y * cameraSensitivity);
                    }
                }
                deltaMouse = Input::GetPrevMousePosition() - Input::GetMousePosition();
            }
        });
    object2->addComponent<Controller>(controller);

    std::shared_ptr<Object> object3 = Object::Create();
    std::shared_ptr<Transform> transform3 = object3->getComponent<Transform>();
    transform3->setPositionY(2.0f);

    object3->addComponent<Mesh>(AssetManager::LoadMesh("data/Meshes/backpack/backpack.obj"));
}

void Editor::update()
{
    if (Input::IsKeyPressed(KeyCode::ESCAPE))
    {
        close();
    }
}
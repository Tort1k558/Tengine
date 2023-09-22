#include "Editor.h"

#include"UISystem.h"
#include"ECS/SystemManager.h"
#include"Components/Transform.h"
#include"Components/Camera.h"
#include"Components/Mesh.h"
#include"Core/Timer.h"
#include"Core/Logger.h"
#include"Core/Input.h"
#include"Scene/SceneManager.h"
#include"Utils/Primitives.h"
#include"Core/AssetManager.h"
#include"Scene/SceneSerializer.h"
#include"Systems/ScriptSystem.h"

namespace Tengine
{
    Editor::Editor(unsigned int width, unsigned int height, const std::string& title) :
        Application(width, height, title)
    {

    }

    void Editor::create()
    {
        UISystem::GetInstance()->setWindow(getWindow());
        SystemManager::AddSystem(UISystem::GetInstance());
        UISystem::GetInstance()->init();
        std::shared_ptr<Scene> scene = Scene::Create();
        SceneManager::SetCurrentScene(scene);

        std::shared_ptr<Object> object = Object::Create();
        std::shared_ptr<Transform> transform = object->getComponent<Transform>();
        transform->setScale({ 10.0f,10.0f,1.0f });
        transform->setPositionY(-1.0f);
        transform->setRotationX(-90.0f);
        object->addComponent(Primitives::CreateSphere(50, 50));

        std::shared_ptr<Object> object2 = Object::Create();
        std::shared_ptr<Camera> camera = Component::Create<Camera>(ProjectionType::Perspective);
        object2->addComponent(camera);
        object2->setName("Camera");
        std::shared_ptr<Transform> transform2 = object2->getComponent<Transform>();
        transform2->setPosition({ 0.0f,0.0f,2.0f });

        ScriptSystem::GetInstance()->addScript(object2, "CameraController");
        std::shared_ptr<Object> object3 = Object::Create();
        std::shared_ptr<Transform> transform3 = object3->getComponent<Transform>();
        transform3->setPositionY(2.0f);

        object3->addComponent(AssetManager::LoadMesh("data/Meshes/backpack/backpack.obj"));
    }

    void Editor::update()
    {
        if (Input::IsKeyPressed(KeyCode::ESCAPE))
        {
            close();
        }
    }
}
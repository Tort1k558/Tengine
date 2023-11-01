#include "Editor.h"

#include<ECS/SystemManager.h>
#include<Components/Transform.h>
#include<Components/Camera.h>
#include<Components/Model.h>
#include<Core/Timer.h>
#include<Core/Logger.h>
#include<Core/Input.h>
#include<Scene/SceneManager.h>
#include<Utils/Primitives.h>
#include<Utils/Mesh.h>
#include<Core/AssetManager.h>
#include<Scene/SceneManager.h>
#include<Systems/ScriptSystem.h>
#include<Systems/RendererSystem.h>

#include"Scripts/ScriptCompiler.h"
#include"Scripts/EditorScriptSystem.h"
#include"UISystem.h"
#include"ProjectManager.h"
#include"FileManager.h"

namespace TengineEditor
{
    Editor::Editor(unsigned int width, unsigned int height, const std::string& title) :
        Application(width, height, title)
    {

    }

    void Editor::create()
    {
        UISystem::GetInstance()->setWindow(getWindow());
        UISystem::GetInstance()->init();
        EditorScriptSystem::GetInstance();
        RendererSystem::GetInstance()->getRendererContext()->enableDebugInfo();

#ifdef NDEBUG
        ScriptCompiler::SetCoreBuildConfiguration(BuildConfiguration::Release);
#else
        ScriptCompiler::SetCoreBuildConfiguration(BuildConfiguration::Debug);
#endif
        RendererSystem::GetInstance()->disableRenderToDefaultFramebuffer();
        
        FileManager::Init();

        ProjectManager::Create("ExampleProject");
        std::shared_ptr<Scene> scene = SceneManager::GetCurrentScene();
        
        std::shared_ptr<Object> object = Object::Create();
        std::shared_ptr<Transform> transform = object->getComponent<Transform>();
        transform->setScale({ 10.0f,10.0f,1.0f });
        transform->setPositionY(-1.0f);
        transform->setRotationX(-90.0f);
        object->addComponent(Component::Create<Model>(Primitives::CreateSphere(50, 50)));
        
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
        
        object3->addComponent(AssetManager::CreateModel("Assets/Meshes/backpack/backpack.obj"));
    }

    void Editor::update()
    {
        UISystem::GetInstance()->update();
        if (Input::IsKeyPressed(KeyCode::ESCAPE))
        {
            ProjectManager::Save();
            destroy();
        }
    }
    void Editor::close()
    {
        UISystem::GetInstance()->destroy();
    }
}
#include "Editor.h"

#include<Components/Transform.h>
#include<Components/Camera.h>
#include<Components/Model.h>
#include<Components/Light.h>
#include<Core/Timer.h>
#include<Core/Logger.h>
#include<Core/Input.h>
#include<Core/AssetManager.h>
#include<Scene/SystemManager.h>
#include<Scene/SceneManager.h>
#include<Scene/SceneManager.h>
#include<Systems/ScriptSystem.h>
#include<Systems/RendererSystem.h>
#include<Utils/Mesh.h>
#include<Utils/Primitives.h>

#include"Scripts/ScriptCompiler.h"
#include"Scripts/EditorScriptSystem.h"
#include"UISystem.h"
#include"ProjectManager.h"

namespace TengineEditor
{
    std::filesystem::path Editor::m_pathToEditor;

    Editor::Editor(unsigned int width, unsigned int height, const std::string& title) :
        Application(width, height, title)
    {

    }

    void Editor::create()
    {
        UISystem::GetInstance()->setWindow(getWindow());
        UISystem::GetInstance()->init();
        EditorScriptSystem::GetInstance();
        RendererSystem::GetInstance()->getRendererContext()->enableFeature(RenderFeature::DebugInfo);

#ifdef NDEBUG
        ScriptCompiler::SetCoreBuildConfiguration(BuildConfiguration::Release);
#else
        ScriptCompiler::SetCoreBuildConfiguration(BuildConfiguration::Debug);
#endif
        RendererSystem::GetInstance()->disableRenderToDefaultFramebuffer();

        m_pathToEditor = std::filesystem::current_path();

        ProjectManager::Load("ExampleProject/ProjectData.project");
        SceneManager::Save(SceneManager::GetCurrentScene());
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

    std::filesystem::path Editor::GetPathToEditor()
    {
        std::string pathToEditor = m_pathToEditor.string();
        std::replace(pathToEditor.begin(), pathToEditor.end(), '\\', '/');
        return pathToEditor;
    }

    std::filesystem::path Editor::GetPathToAssets()
    {
        if (ProjectManager::GetInstance())
        {
            return "Assets";
        }
        return "";
    }
}
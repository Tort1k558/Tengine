#include "ProjectManager.h"

#include<fstream>
#include<nlohmann/json.hpp>
#include<Scene/SceneManager.h>
#include<Systems/ScriptSystem.h>

#include"Scripts/EditorScriptSystem.h"

#include"UISystem.h"

namespace TengineEditor
{
	std::shared_ptr<Project> ProjectManager::m_instance;

    std::shared_ptr<Project> ProjectManager::GetInstance()
    {
		return m_instance;
    }

    std::shared_ptr<Project> ProjectManager::Load(std::filesystem::path path)
    {
		std::ifstream file(path);
		if (file.is_open())
		{
			nlohmann::json data = nlohmann::json::parse(file);
			std::filesystem::current_path(path.parent_path());

			EditorScriptSystem::GetInstance()->setPathToDll("builds/ScriptModule/ScriptModule.dll");
			EditorScriptSystem::GetInstance()->reload();

			m_instance = std::shared_ptr<Project>(new Project());
			m_instance->setPath(path.parent_path());
			m_instance->m_name = data["name"];
			m_instance->m_pathToScenes = data["scenes"];
			for (const auto& pathToScene : m_instance->getPathToScenes())
			{
				SceneManager::AddScene(pathToScene.filename().string(), pathToScene.string());
			}
			if (!m_instance->getPathToScenes().empty())
			{
				SceneManager::LoadByPath(m_instance->m_pathToScenes[0].string());
			}

			if (data.find("EditorCamera") != data.end())
			{
				std::shared_ptr<Object> sceneCamera = std::make_shared<Object>();
				sceneCamera->addComponent(Component::Create<Transform>());
				SceneManager::DeserializeObject(data["EditorCamera"], sceneCamera);
				UISystem::GetInstance()->setSceneCamera(sceneCamera);
			}
			else
			{
				std::shared_ptr<Object> sceneCamera = std::make_shared<Object>();
				sceneCamera->setName("SceneCamera");
				sceneCamera->addComponent(Component::Create<Transform>());
				sceneCamera->addComponent(Component::Create<Camera>());
				UISystem::GetInstance()->setSceneCamera(sceneCamera);
			}
			return m_instance;
		}
		else
		{
			Logger::Critical("ERROR::ProjectManager::Failed to load project!");
		}
		return nullptr;
    }

    std::shared_ptr<Project> ProjectManager::Create(std::filesystem::path path)
    {
		EditorScriptSystem::GetInstance()->setPathToDll("builds/ScriptModule/ScriptModule.dll");
		EditorScriptSystem::GetInstance()->reload();
		m_instance = std::shared_ptr<Project>(new Project());
		m_instance->m_name = path.filename().string();
		m_instance->setPath(path);
		std::filesystem::current_path(path);
		std::filesystem::create_directory("Assets");
		std::shared_ptr<Scene> defaultScene = Scene::Create();
		defaultScene->setName("DefaultScene");
		defaultScene->setPath("Assets/DefaultScene.scene");
		m_instance->addScene(defaultScene->getPath());
		SceneManager::Save(defaultScene);
		SceneManager::SetCurrentScene(defaultScene);

		std::shared_ptr<Object> sceneCamera = std::make_shared<Object>();
		sceneCamera->setName("SceneCamera");
		sceneCamera->addComponent(Component::Create<Transform>());
		sceneCamera->addComponent(Component::Create<Camera>());
		UISystem::GetInstance()->setSceneCamera(sceneCamera);

		Save();
		return m_instance;
    }

	void ProjectManager::Save()
	{
		SceneManager::Save(SceneManager::GetCurrentScene());
		nlohmann::json data;
		data["name"] = m_instance->getName();
		data["scenes"] = m_instance->getPathToScenes();
		std::shared_ptr<Object> sceneCamera = UISystem::GetInstance()->getSceneCamera();
		if (sceneCamera)
		{
			SceneManager::SerializeObject(data["EditorCamera"], sceneCamera);
		}

		std::ofstream file("ProjectData.project");
		if (file.is_open()) {
			file << data.dump(4);
			file.close();
		}
		else {
			Logger::Critical("ERROR::Serializer::Failed to open the file for writing!");
		}
	}
}


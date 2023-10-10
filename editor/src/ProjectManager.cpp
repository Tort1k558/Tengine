#include "ProjectManager.h"

#include<fstream>
#include<nlohmann/json.hpp>
#include<Scene/SceneManager.h>
#include<Systems/ScriptSystem.h>


namespace TengineEditor
{
	std::shared_ptr<Project> ProjectManager::m_instance;

    std::shared_ptr<Project> ProjectManager::GetInstance()
    {
		if (!m_instance)
		{
			m_instance = Create("Project");
		}
		return m_instance;
    }

    std::shared_ptr<Project> ProjectManager::Load(std::filesystem::path path)
    {
		m_instance = std::shared_ptr<Project>(new Project());
		m_instance->setPath(path.parent_path());
		std::ifstream file(path);
		nlohmann::json data = nlohmann::json::parse(file);
		m_instance->m_name = data["name"];
		m_instance->m_scenes = data["scenes"];
		for (const auto& scene : m_instance->m_scenes)
		{
			SceneManager::AddScene(scene.filename().string(), m_instance->getPath().string()+ "/" + scene.string());
		}
		if (!m_instance->m_scenes.empty())
		{
			SceneManager::LoadByPath(m_instance->getPath().string() + "/" + m_instance->m_scenes[0].string());
		}
		ScriptSystem::GetInstance()->setPathToDll(m_instance->getPath().string() + "/build/ScriptModule/ScriptModule.dll");
		ScriptSystem::GetInstance()->reload();
		return m_instance;
    }

    std::shared_ptr<Project> ProjectManager::Create(std::filesystem::path path)
    {
		m_instance = std::shared_ptr<Project>(new Project());
		m_instance->m_name = path.filename().string();
		m_instance->setPath(path);
		std::shared_ptr<Scene> defaultScene = Scene::Create();
		defaultScene->setName("DefaultScene");
		defaultScene->setPath(m_instance->getPath().string() + "/DefaultScene.scene");
		m_instance->addScene("DefaultScene.scene");
		SceneManager::Save(defaultScene);
		SceneManager::SetCurrentScene(defaultScene);
		Save();
		ScriptSystem::GetInstance()->setPathToDll(m_instance->getPath().string() + "/build/ScriptModule/ScriptModule.dll");
		ScriptSystem::GetInstance()->reload();
		return m_instance;
    }

	void ProjectManager::Save()
	{
		SceneManager::Save(SceneManager::GetCurrentScene());
		nlohmann::json data;
		data["name"] = m_instance->getName();
		data["scenes"] = m_instance->m_scenes;
		std::ofstream file(m_instance->getPath().string() + "/ProjectData.project");
		if (file.is_open()) {
			file << data.dump(4);
			file.close();
		}
		else {
			Logger::Critical("ERROR::Serializer::Failed to open the file for writing!");
		}
	}
}


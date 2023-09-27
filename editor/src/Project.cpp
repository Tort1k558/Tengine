#include "Project.h"

#include<fstream>

#include"Scene/SceneManager.h"
namespace TengineEditor
{
	using namespace Tengine;

	std::shared_ptr<Project> Project::m_instance;

	std::shared_ptr<Project> Project::GetInstance()
	{
		if (!m_instance)
		{
			m_instance = CreateProject("Project");
		}
		return m_instance;
	}

	std::shared_ptr<Project> Project::LoadProject(std::filesystem::path path)
	{
		m_instance = std::shared_ptr<Project>(new Project());
		m_instance->m_path = path.root_directory();
		std::ifstream file(path);
		nlohmann::json data = nlohmann::json::parse(file);
		m_instance->m_name = data["name"];
		m_instance->m_scenes = data["scenes"];
		for (const auto& scene : data["scenes"])
		{
			m_instance->addScene(scene);
		}
		if(!m_instance->m_scenes.empty())
		{
			SceneManager::LoadByPath(m_instance->m_scenes[0]);
		}
		return m_instance;
	}

	std::shared_ptr<Project> Project::CreateProject(std::filesystem::path path)
	{
		m_instance = std::shared_ptr<Project>(new Project());
		m_instance->m_name = "Project";
		m_instance->m_path = path;
		std::shared_ptr<Scene> defaultScene = Scene::Create();
		defaultScene->setName("DefaultScene");
		defaultScene->setPath(m_instance->m_path.string() + "/" + defaultScene->getName() + ".scene");
		m_instance->addScene(defaultScene->getPath().string());
		SceneManager::Save(defaultScene);
		SceneManager::SetCurrentScene(defaultScene);
		m_instance->save();
		return m_instance;
	}

	void Project::save()
	{
		nlohmann::json data;
		data["name"] = m_name;
		for (const auto& scene : m_scenes)
		{
			data["scenes"] = scene;
		}
		std::ofstream file(m_path.string() +"/" + m_name + ".project");
		if (file.is_open()) {
			file << data.dump(4);
			file.close();
		}
		else {
			Logger::Critical("ERROR::Serializer::Failed to open the file for writing!");
		}
	}

	std::filesystem::path Project::getPath()
	{
		return m_path;
	}

	std::string Project::getName()
	{
		return m_name;
	}

	void Project::removeScene(std::shared_ptr<Scene> scene)
	{
		removeScene(scene->getName());
	}

	void Project::removeScene(const std::string& nameScene)
	{
		for (size_t i = 0; i < m_scenes.size(); i++)
		{
			if (m_scenes[i].filename() == nameScene)
			{
				m_scenes.erase(m_scenes.begin() + i);
				return;
			}
		}
	}

	void Project::setName(const std::string& name)
	{
		m_name = name;
	}

	void Project::addScene(std::filesystem::path path)
	{
		m_scenes.push_back(path);
		SceneManager::AddScene(path.filename().string(),path);
	}
}
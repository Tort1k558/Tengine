#include "Project.h"

#include<fstream>
#include<Scene/SceneManager.h>

namespace TengineEditor
{
	using namespace Tengine;

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
		removeSceneByName(scene->getName());
	}

	void Project::removeSceneByName(std::string_view nameScene)
	{
		for (size_t i = 0; i < m_pathToScenes.size(); i++)
		{
			if (m_pathToScenes[i].stem() == nameScene)
			{
				m_pathToScenes.erase(m_pathToScenes.begin() + i);
				return;
			}
		}
	}

	void Project::removeSceneByPath(std::filesystem::path path)
	{
		for (size_t i = 0; i < m_pathToScenes.size(); i++)
		{
			if (m_pathToScenes[i] == path)
			{
				m_pathToScenes.erase(m_pathToScenes.begin() + i);
				return;
			}
		}
	}
	void Project::swapScenes(size_t index1, size_t index2)
	{
		std::iter_swap(m_pathToScenes.begin() + index1, m_pathToScenes.begin() + index2);
	}

	std::vector<std::filesystem::path> Project::getPathToScenes()
	{
		return m_pathToScenes;
	}

	void Project::setPath(std::filesystem::path path)
	{
		std::string sPath = path.string();
		std::replace(sPath.begin(), sPath.end(), '\\', '/');
		m_path = sPath;
	}

	void Project::setName(std::string_view name)
	{
		m_name = name;
	}

	void Project::addScene(std::filesystem::path path)
	{
		if (std::filesystem::exists(path) && std::find(m_pathToScenes.begin(), m_pathToScenes.end(), path) == m_pathToScenes.end())
		{
			m_pathToScenes.push_back(path);
			SceneManager::AddScene(path.stem().string(), getPath().string() + path.string());
		}
	}
}
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
		removeScene(scene->getName());
	}

	void Project::removeScene(const std::string& nameScene)
	{
		for (size_t i = 0; i < m_pathToScenes.size(); i++)
		{
			if (m_pathToScenes[i].filename() == nameScene)
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

	void Project::setName(const std::string& name)
	{
		m_name = name;
	}

	void Project::addScene(std::filesystem::path path)
	{
		m_pathToScenes.push_back(path);
		SceneManager::AddScene(path.filename().string(),getPath().string() + path.string());
	}
}
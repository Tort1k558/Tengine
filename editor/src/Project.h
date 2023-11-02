#pragma once

#include<vector>
#include<string>
#include<memory>
#include<filesystem>

#include<Scene/Scene.h>

namespace TengineEditor
{
	using namespace Tengine;

	class Project
	{
	public:
		void setName(std::string_view name);
		std::string getName();
		void addScene(std::filesystem::path path);
		std::filesystem::path getPath();
		void removeScene(std::shared_ptr<Scene> scene);
		void removeSceneByName(std::string_view nameScene);
		void removeSceneByPath(std::filesystem::path path);
		void swapScenes(size_t index1, size_t index2);

		std::vector<std::filesystem::path> getPathToScenes();
	private:
		void setPath(std::filesystem::path path);
		Project() = default;
		Project(const Project&) = delete;
		Project& operator=(const Project&) = delete;
		
		std::filesystem::path m_path;
		std::vector<std::filesystem::path> m_pathToScenes;
		std::string m_name;

		friend class ProjectManager;
	};
}
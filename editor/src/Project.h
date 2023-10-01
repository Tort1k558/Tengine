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
		void setName(const std::string& name);
		void addScene(std::filesystem::path path);
		std::filesystem::path getPath();
		std::string getName();
		void removeScene(std::shared_ptr<Scene> scene);
		void removeScene(const std::string& nameScene);
		void swapScenes(size_t index1, size_t index2);
		std::vector<std::filesystem::path> getPathToScenes();
	private:
		void setPath(std::filesystem::path path);
		Project() = default;
		Project(const Project&) = delete;
		Project& operator=(const Project&) = delete;
		
		std::filesystem::path m_path;
		std::vector<std::filesystem::path> m_scenes;
		std::string m_name;

		friend class ProjectManager;
	};
}
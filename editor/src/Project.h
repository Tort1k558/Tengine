#pragma once

#include<vector>
#include<string>
#include<memory>
#include<filesystem>

#include"Scene/Scene.h"
namespace TengineEditor
{
	using namespace Tengine;

	class Project
	{
	public:
		static std::shared_ptr<Project> GetInstance();
		static std::shared_ptr<Project> LoadProject(std::filesystem::path path);
		static std::shared_ptr<Project> CreateProject(std::filesystem::path path);
		void save();
		void setName(const std::string& name);
		void addScene(std::filesystem::path path);
		std::filesystem::path getPath();
		std::string getName();
		void removeScene(std::shared_ptr<Scene> scene);
		void removeScene(const std::string& nameScene);
	private:
		Project() = default;
		Project(const Project&) = delete;
		Project& operator=(const Project&) = delete;

		static std::shared_ptr<Project> m_instance;
		
		std::filesystem::path m_path;
		std::vector<std::filesystem::path> m_scenes;
		std::string m_name;
	};
}
#pragma once

#include<memory>

#include"Project.h"
namespace TengineEditor
{
	class ProjectManager
	{
	public:
		ProjectManager() = delete;

		static std::shared_ptr<Project> GetInstance();
		static std::shared_ptr<Project> Load(std::filesystem::path path);
		static std::shared_ptr<Project> Create(std::filesystem::path path);
		static void Save();
	private:
		static std::shared_ptr<Project> m_instance;
	};
}
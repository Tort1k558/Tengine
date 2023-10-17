#include "FileManager.h"

#include<fstream>
#include<Core/Logger.h>

#include"ProjectManager.h"
namespace TengineEditor
{
	std::filesystem::path FileManager::m_relativePath;

	void FileManager::NewFolder(std::string_view name)
	{
		std::filesystem::path path = GetCurrentPath().string() + "/" + name.data();
		for (size_t i = 0; std::filesystem::exists(path); i++)
		{
			path = GetCurrentPath().string() + "/" + name.data() + std::to_string(i);
		}
		std::filesystem::create_directory(path);
	}

	void FileManager::NewFile(std::string_view name, std::string_view data)
	{
		std::filesystem::path path = GetCurrentPath().string() + "/" + name.data();
		for (size_t i = 0; std::filesystem::exists(path); i++)
		{
			path = GetCurrentPath().string() + "/" + name.data() + std::to_string(i);
		}
		std::ofstream file(path);
		if (file.is_open())
		{
			file << data;

			file.close();
		}
		else
		{
			Logger::Critical("ERROR::FileManager::Failed to create file::{0}", name.data());
		}
	}
	
	void FileManager::RemoveFile(std::string_view name)
	{
		std::filesystem::path path = GetCurrentPath().string() + "/" + name.data();
		std::filesystem::remove(path);
	}
	
	void FileManager::SetRelativePath(std::filesystem::path path)
	{
		m_relativePath = path;
	}

	std::filesystem::path FileManager::GetCurrentPath()
	{
		if (ProjectManager::GetInstance())
		{
			return ProjectManager::GetInstance()->getPath().string() + "/Assets/" + m_relativePath.string();
		}
		return "";
	}

	std::filesystem::path FileManager::GetRelativePath()
	{
		return m_relativePath;
	}

	std::vector<std::filesystem::path> FileManager::GetAllProjectFilePaths()
	{
		std::vector<std::filesystem::path> allProjectFilePaths;
		if (ProjectManager::GetInstance())
		{
			for (const auto& entry : std::filesystem::directory_iterator(ProjectManager::GetInstance()->getPath().string() + "/Assets/")) {
				allProjectFilePaths.push_back(entry.path());
			}
		}
		return allProjectFilePaths;
	}
	std::vector<std::filesystem::path> FileManager::GetAllProjectFileFromCurrentPath()
	{
		std::vector<std::filesystem::path> allProjectFilePaths;
		for (const auto& entry : std::filesystem::directory_iterator(GetCurrentPath()))
		{
			allProjectFilePaths.push_back(entry.path());
		}
		return allProjectFilePaths;
	}
}
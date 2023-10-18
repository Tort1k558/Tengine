#include "FileManager.h"

#include<fstream>
#include<Core/Logger.h>

#include"ProjectManager.h"
namespace TengineEditor
{
	std::filesystem::path FileManager::m_relativePath;

	void FileManager::NewFolder(std::filesystem::path path)
	{
		std::filesystem::path pathToFile = GetPathToAssets().string() + "/" + path.string();
		for (size_t i = 0; std::filesystem::exists(path); i++)
		{
			pathToFile = GetPathToAssets().string() + "/" + path.string() + std::to_string(i);
		}
		std::filesystem::create_directory(pathToFile);
	}

	void FileManager::NewFile(std::filesystem::path path, std::string_view data)
	{
		std::filesystem::path pathToFile = GetPathToAssets().string() + "/" + path.string();
		for (size_t i = 0; std::filesystem::exists(pathToFile); i++)
		{
			pathToFile = GetPathToAssets().string() + "/" + path.string() + std::to_string(i);
		}
		std::ofstream file(pathToFile);
		if (file.is_open())
		{
			file << data;

			file.close();
		}
		else
		{
			Logger::Critical("ERROR::FileManager::Failed to create file::{0}", path.string());
		}
	}
	
	void FileManager::RemoveFile(std::filesystem::path path)
	{
		std::filesystem::path pathToFile = GetPathToAssets().string() + "/" + path.string();
		std::filesystem::remove(pathToFile);
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

	std::filesystem::path FileManager::GetPathToAssets()
	{
		if (ProjectManager::GetInstance())
		{
			return ProjectManager::GetInstance()->getPath().string() + "/Assets";
		}
		return "";
	}

	std::filesystem::path FileManager::GetRelativePath()
	{
		return m_relativePath;
	}

	std::vector<std::filesystem::path> FileManager::GetAllProjectFiles()
	{
		std::vector<std::filesystem::path> allProjectFilePaths;
		if (ProjectManager::GetInstance())
		{
			for (const auto& entry : std::filesystem::directory_iterator(GetPathToAssets())) 
			{
				allProjectFilePaths.push_back(entry.path().filename());
			}
		}
		return allProjectFilePaths;
	}
	std::vector<std::filesystem::path> FileManager::GetAllFileFromCurrentPath()
	{
		std::vector<std::filesystem::path> allProjectFilePaths;
		for (const auto& entry : std::filesystem::directory_iterator(GetCurrentPath()))
		{
			allProjectFilePaths.push_back(std::filesystem::relative(entry.path(),GetPathToAssets()));
		}
		return allProjectFilePaths;
	}
}
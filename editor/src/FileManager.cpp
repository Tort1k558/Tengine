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
		for (size_t i = 0; std::filesystem::exists(pathToFile); i++)
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
		std::filesystem::remove_all(pathToFile);
	}

	void FileManager::RenameFile(std::filesystem::path path, std::string_view name)
	{
		std::filesystem::path pathToFile = GetPathToAssets().string() + "/" + path.string();
		std::filesystem::rename(pathToFile, pathToFile.parent_path().string() + "/" + name.data());
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
		std::vector<std::filesystem::path> filePaths;
		if (ProjectManager::GetInstance())
		{
			for (const auto& entry : std::filesystem::directory_iterator(GetPathToAssets())) 
			{
				filePaths.push_back(entry.path().filename());
			}
		}
		return filePaths;
	}
	
	std::vector<std::filesystem::path> FileManager::GetFilesFromCurrentDirectory(std::filesystem::path path)
	{
		std::vector<std::filesystem::path> filePaths;
		if (ProjectManager::GetInstance())
		{
			for (const auto& entry : std::filesystem::directory_iterator(GetPathToAssets().string() + "/" + path.string()))
			{
				std::string strPath = std::filesystem::relative(entry.path(), GetPathToAssets().string()).string();
				std::replace(strPath.begin(), strPath.end(), '\\', '/');
				filePaths.push_back(strPath);
			}
		}
		return filePaths;
	}
}
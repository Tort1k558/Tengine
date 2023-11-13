#include "FileManager.h"

#include<fstream>
#include<Core/Logger.h>

#include"ProjectManager.h"
namespace TengineEditor
{
	std::filesystem::path FileManager::m_relativePath;
	std::filesystem::path FileManager::m_pathToEditor;

	void FileManager::Init()
	{
		m_pathToEditor = std::filesystem::current_path();
		m_relativePath = "Assets";
	}

	void FileManager::NewFolder(std::filesystem::path path)
	{
		std::filesystem::path pathToFile = GetUniqueFilePath(path.string());
		std::filesystem::create_directory(pathToFile);
	}

	void FileManager::NewFile(std::filesystem::path path, std::string_view data)
	{
		std::filesystem::path pathToFile = GetUniqueFilePath(path.string());
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
		std::filesystem::remove_all(path);
	}

	void FileManager::RenameFile(std::filesystem::path path, std::string_view name)
	{
		std::filesystem::rename(path, path.parent_path().string() + "/" + name.data());
	}
	
	void FileManager::SetRelativePath(std::filesystem::path path)
	{
		m_relativePath = path;
	}

	std::filesystem::path FileManager::GetCurrentPath()
	{
		if (ProjectManager::GetInstance())
		{
			return m_relativePath.string();
		}
		return "";
	}

	std::filesystem::path FileManager::GetPathToAssets()
	{
		if (ProjectManager::GetInstance())
		{
			return "Assets";
		}
		return "";
	}

	std::filesystem::path FileManager::GetRelativePath()
	{
		return m_relativePath;
	}

	std::filesystem::path FileManager::GetPathToEditor()
	{
		std::string pathToEditor = m_pathToEditor.string();
		std::replace(pathToEditor.begin(), pathToEditor.end(), '\\', '/');
		return pathToEditor;
	}
	
	std::vector<std::filesystem::path> FileManager::GetFilesFromCurrentDirectory(std::filesystem::path path)
	{
		std::vector<std::filesystem::path> filePaths;
		if (ProjectManager::GetInstance())
		{
			if (path.empty())
			{
				path = "Assets";
			}
			for (const auto& entry : std::filesystem::directory_iterator(path.string()))
			{
				std::string strPath = entry.path().string();
				std::replace(strPath.begin(), strPath.end(), '\\', '/');
				filePaths.push_back(strPath);
			}
		}
		return filePaths;
	}
	std::filesystem::path FileManager::GetUniqueFilePath(std::filesystem::path originalPath)
	{
		std::filesystem::path pathToFile = originalPath;

		for (size_t i = 0; std::filesystem::exists(pathToFile); i++) {
			pathToFile =  originalPath.parent_path().string() + "/" + originalPath.stem().string() + std::to_string(i) + originalPath.extension().string();
		}

		return pathToFile;
	}
}
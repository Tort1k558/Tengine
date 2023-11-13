#pragma once

#include<filesystem>

namespace TengineEditor
{
	class FileManager
	{
	public:
		FileManager() = delete;
		
		static void Init();
		static void NewFolder(std::filesystem::path name);
		static void NewFile(std::filesystem::path path, std::string_view data = "");
		static void RemoveFile(std::filesystem::path path);
		static void RenameFile(std::filesystem::path path, std::string_view name);
		static void SetRelativePath(std::filesystem::path path);
		static std::filesystem::path GetCurrentPath();
		static std::filesystem::path GetPathToAssets();
		static std::filesystem::path GetRelativePath();
		static std::filesystem::path GetPathToEditor();
		static std::vector<std::filesystem::path> GetFilesFromCurrentDirectory(std::filesystem::path path);
		static std::filesystem::path GetUniqueFilePath(std::filesystem::path originalPath);
	private:
		static std::filesystem::path m_relativePath;
		static std::filesystem::path m_pathToEditor;
	};
}
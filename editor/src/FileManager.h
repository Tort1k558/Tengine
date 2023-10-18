#pragma once

#include<filesystem>

namespace TengineEditor
{
	class FileManager
	{
	public:
		FileManager() = delete;

		static void NewFolder(std::filesystem::path name);
		static void NewFile(std::filesystem::path path, std::string_view data = "");
		static void RemoveFile(std::filesystem::path path);
		static void SetRelativePath(std::filesystem::path path);
		static std::filesystem::path GetCurrentPath();
		static std::filesystem::path GetPathToAssets();
		static std::filesystem::path GetRelativePath();
		static std::vector<std::filesystem::path> GetAllProjectFiles();
		static std::vector<std::filesystem::path> GetAllFileFromCurrentPath();
	private:
		static std::filesystem::path m_relativePath;
	};
}
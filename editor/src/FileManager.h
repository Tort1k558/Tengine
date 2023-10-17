#pragma once

#include<filesystem>

namespace TengineEditor
{
	class FileManager
	{
	public:
		FileManager() = delete;

		static void NewFolder(std::string_view name);
		static void NewFile(std::string_view name, std::string_view data = "");
		static void RemoveFile(std::string_view name);
		static void SetRelativePath(std::filesystem::path path);
		static std::filesystem::path GetCurrentPath();
		static std::filesystem::path GetRelativePath();
		static std::vector<std::filesystem::path> GetAllProjectFilePaths();
		static std::vector<std::filesystem::path> GetAllProjectFileFromCurrentPath();
	private:
		static std::filesystem::path m_relativePath;
	};
}
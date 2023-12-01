#pragma once

#include<filesystem>
#include"Core/Core.h"

namespace Tengine
{
	class TENGINE FileManager
	{
	public:
		FileManager() = delete;
		
		static void NewFolder(std::filesystem::path name);
		static void NewFile(std::filesystem::path path, std::string_view data = "");
		static void RemoveFile(std::filesystem::path path);
		static void RenameFile(std::filesystem::path path, std::string_view name);
		static std::vector<std::filesystem::path> GetFilesFromCurrentDirectory(std::filesystem::path path);
		static std::filesystem::path GetUniqueFilePath(std::filesystem::path originalPath);
	};
}
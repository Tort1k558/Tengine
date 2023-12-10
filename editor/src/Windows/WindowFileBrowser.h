#pragma once

#include<Scene/Object.h>

namespace TengineEditor
{
	using namespace Tengine;


	class WindowFileBrowser
	{
	public:
		WindowFileBrowser() = delete;

		static void Render();
	private:
		static std::filesystem::path GetCurrentPath();
		static void SetRelativePath(std::filesystem::path path);
		static std::filesystem::path GetRelativePath();

		static std::filesystem::path m_relativePath;

	};

	
}
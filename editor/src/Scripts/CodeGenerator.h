#pragma once

#include<filesystem>
#include<unordered_set>

namespace Tengine
{
	struct ClassInfo
	{
		std::string className;
	};
	class CodeGenerator
	{
	public:
		static void UpdateScripts();
		static void AddScript(std::filesystem::path pathToScript);
	private:
		static void GeneratePremake();
		static void BuildDll();
		static void GenerateInitFiles();
		static std::unordered_set<std::filesystem::path> m_pathsToScripts;

	};
}
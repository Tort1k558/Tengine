#pragma once

#include<filesystem>
#include<unordered_set>

namespace Tengine
{
	struct ScriptInfo
	{
		std::string name;
		std::filesystem::path path;
	};
	class CodeGenerator
	{
	public:
		static void CompileScripts();
	private:
		static void FindAllScripts();
		static void GenerateCmake();
		static void BuildDll();
		static void GenerateInitFiles();
		static std::vector<ScriptInfo> m_scriptInfo;
	};
}
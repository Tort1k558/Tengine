#pragma once

#include<filesystem>
#include<unordered_set>
 
#include"ProjectBuilder.h"

namespace TengineEditor
{
	struct FieldScriptInfo
	{
		std::string type;
		std::string name;
	};

	struct ScriptInfo
	{
		std::string name;
		std::filesystem::path path;
		std::vector<FieldScriptInfo> fields;
	};
	class ScriptCompiler
	{
	public:

		static void Compile();
		static void SetScriptBuildConfiguration(BuildConfiguration config);
		static void SetCoreBuildConfiguration(BuildConfiguration config);
		static BuildConfiguration GetScriptBuildConfiguration();
		static BuildConfiguration GetCoreBuildConfiguration();
		static std::vector<ScriptInfo> GetScriptInfo();
		static std::string GetMetaInfo();
	private:

		static void GenerateCmake();
		static void GenerateInitFiles();
		static void BuildDll();

		static BuildConfiguration m_scriptBuildConfiguration;
		static BuildConfiguration m_coreBuildConfiguration;
	};
}
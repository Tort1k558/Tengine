#pragma once

#include<filesystem>
#include<unordered_set>
 
#include"ProjectBuilder.h"

namespace TengineEditor
{

	class ScriptCompiler
	{
	public:
		static void Compile();
		static void SetScriptBuildConfiguration(BuildConfiguration config);
		static void SetCoreBuildConfiguration(BuildConfiguration config);
		static BuildConfiguration GetScriptBuildConfiguration();
		static BuildConfiguration GetCoreBuildConfiguration();
	private:
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

		static void GetScriptInfo();
		static void GenerateMetaData();
		static void GenerateCmake();
		static void GenerateInitFiles();
		static void BuildDll();

		static std::vector<ScriptInfo> m_scriptInfo;
		static std::string m_metaData;

		static BuildConfiguration m_scriptBuildConfiguration;
		static BuildConfiguration m_coreBuildConfiguration;
	};
}
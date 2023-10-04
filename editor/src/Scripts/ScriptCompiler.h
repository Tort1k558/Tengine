#pragma once

#include<filesystem>
#include<unordered_set>
 

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
	private:
		static void GetScriptInfo();
		static void GenerateMetaData();
		static void GenerateCmake();
		static void GenerateInitFiles();
		static void BuildDll();
		static std::vector<ScriptInfo> m_scriptInfo;
		static std::string m_metaData;
	};
}
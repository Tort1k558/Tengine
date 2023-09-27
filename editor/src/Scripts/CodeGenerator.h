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
	class CodeGenerator
	{
	public:
		static void CompileScripts();
	private:
		static void GetScriptInfo();
		static void GenerateMetaData();
		static void GenerateCmake();
		static void BuildDll();
		static void GenerateInitFiles();
		static std::vector<ScriptInfo> m_scriptInfo;
		static std::string m_metaData;
	};
}
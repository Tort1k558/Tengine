#pragma once

#include<filesystem>

namespace TengineEditor
{
	class ProjectBuilderHTML5
	{
	public:
		static void Build();
	private:
		static void GenerateInitFiles();
		static void GenerateCMake();
		static void BuildSolution();
		static void CollectFiles();

		static std::filesystem::path m_pathToBuildDirectory;
	};
}
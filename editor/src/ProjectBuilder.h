#pragma once

namespace TengineEditor
{
	enum class BuildConfiguration
	{
		Debug = 0,
		Release
	};
	class ProjectBuilder
	{
	public:
		ProjectBuilder() = delete;
		static void Build();
		static void SetBuildConfiguration(BuildConfiguration config);
		static BuildConfiguration GetBuildConfiguration();
		
	private:
		static void GenerateInitFiles();
		static void GenerateCMake();
		static void BuildSolution();
		static void CollectFiles();

		static BuildConfiguration m_buildConfiguration;
	};
}
#pragma once

namespace TengineEditor
{
	enum class BuildConfiguration
	{
		Debug = 0,
		Release
	};

	enum class BuildPlatform
	{
		Windows = 0,
		HTML5
	};

	class ProjectBuilder
	{
	public:
		ProjectBuilder() = delete;
		static void Build();
		static void SetBuildConfiguration(BuildConfiguration config);
		static void SetBuildPlatform(BuildPlatform platform);
		static BuildConfiguration GetBuildConfiguration();
		static BuildPlatform GetBuildPlatform();
		
	private:
		static void GenerateInitFiles();
		static void GenerateCMake();
		static void BuildSolution();
		static void CollectFiles();

		static BuildPlatform m_buildPlatform;
		static BuildConfiguration m_buildConfiguration;
	};
}
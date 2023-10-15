#include "ProjectBuilder.h"

#include"ProjectBuilderWindows.h"
#include"ProjectBuilderHTML5.h"

namespace TengineEditor
{
	BuildConfiguration ProjectBuilder::m_buildConfiguration = BuildConfiguration::Debug;
	BuildPlatform ProjectBuilder::m_buildPlatform = BuildPlatform::Windows;

	void ProjectBuilder::Build()
	{
		switch (m_buildPlatform)
		{
		case BuildPlatform::Windows:
			ProjectBuilderWindows::Build();
			break;
		case BuildPlatform::HTML5:
			ProjectBuilderHTML5::Build();
			break;
		default:
			break;
		}
	}

	void ProjectBuilder::SetBuildConfiguration(BuildConfiguration config)
	{
		m_buildConfiguration = config;
	}

	void ProjectBuilder::SetBuildPlatform(BuildPlatform platform)
	{
		m_buildPlatform = platform;
	}

	BuildConfiguration ProjectBuilder::GetBuildConfiguration()
	{
		return m_buildConfiguration;
	}

	BuildPlatform ProjectBuilder::GetBuildPlatform()
	{
		return m_buildPlatform;
	}
}

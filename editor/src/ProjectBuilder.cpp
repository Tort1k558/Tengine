#include "ProjectBuilder.h"

#include<fstream>
#include<Core/Logger.h>

#include"ProjectManager.h"
#include"Scripts/ScriptCompiler.h"

namespace TengineEditor
{
	BuildConfiguration ProjectBuilder::m_buildConfiguration = BuildConfiguration::Debug;

	void ProjectBuilder::Build()
	{
		ScriptCompiler::Compile();
		std::string pathToProject = ProjectManager::GetInstance()->getPath().string();
		std::filesystem::create_directory(pathToProject + "/build");
		std::filesystem::create_directory(pathToProject + "/build/Game");
		std::filesystem::create_directory(pathToProject + "/build/Game/bin");
		GenerateInitFiles();
		GenerateCMake();
		BuildSolution();
		CollectFiles();
	}
	void ProjectBuilder::SetBuildConfiguration(BuildConfiguration config)
	{
		m_buildConfiguration = config;
	}
	BuildConfiguration ProjectBuilder::GetBuildConfiguration()
	{
		return m_buildConfiguration;
	}

	void ProjectBuilder::GenerateInitFiles()
	{
        //Source file
		std::string projectName = ProjectManager::GetInstance()->getName();
		std::ofstream gameHeaderFile(ProjectManager::GetInstance()->getPath().string() + "/build/Game/Game.h");
		if (gameHeaderFile.is_open())
		{
			gameHeaderFile << R"(#include<Core/Application.h>

using namespace Tengine;

class Game : public Application
{
public:
    Game(unsigned int width, unsigned int height, const std::string& title);
    void create() final;
    void update() final;
    void close() final;
};
)";
			gameHeaderFile.close();
		}
		std::ofstream gameCppFile(ProjectManager::GetInstance()->getPath().string() + "/build/Game/Game.cpp");
		if (gameCppFile.is_open())
		{
			gameCppFile << R"(#pragma once
#include"Game.h"

#include<Systems/ScriptSystem.h>
#include<Scene/SceneManager.h>
#include<ECS/SystemManager.h>

Game::Game(unsigned int width, unsigned int height, const std::string& title) :
    Application(width, height, title)
{

}

void Game::create()
{
	ScriptSystem::GetInstance()->setPathToDll("scriptModule.dll");
	ScriptSystem::GetInstance()->reload();)";
			std::vector<std::filesystem::path> pathToScenes = ProjectManager::GetInstance()->getPathToScenes();
			for (const auto& scene : pathToScenes)
			{
				gameCppFile << "SceneManager::AddScene(\"" + scene.stem().string() + "\",\"" + scene.string() + "\");\n\t";
			}
			gameCppFile << "SceneManager::LoadByPath(\"" + pathToScenes[0].string() + "\");\n";
			gameCppFile << R"(
	SystemManager::AddSystem(ScriptSystem::GetInstance());
}

void Game::update()
{
    
}

void Game::close()
{
    
}
)";
			gameHeaderFile.close();
		}
		else
		{
			Logger::Critical("ERROR::ProjectBuilder::Error creating game header file");
		}
        std::ofstream mainFile(ProjectManager::GetInstance()->getPath().string() + "/build/Game/main.cpp");
        if (mainFile.is_open())
        {
			mainFile << \
                R"(#include"Game.h"

int main(int argc, char** argv)
{
    Game Game(1024, 768,")" + projectName + R"(");
    Game.init();
    Game.run();

    return 0;
}
)";
			mainFile.close();
        }
        else
        {
            Logger::Critical("ERROR::ProjectBuilder::Error creating main file");
        }
	}

	void ProjectBuilder::GenerateCMake()
	{
		std::string projectName = ProjectManager::GetInstance()->getName();
		std::ofstream cmakeFile(ProjectManager::GetInstance()->getPath().string() + "/build/Game/CMakeLists.txt");
		if (cmakeFile.is_open())
		{
			std::string pathToEditor = std::filesystem::current_path().string();
			std::replace(pathToEditor.begin(), pathToEditor.end(), '\\', '/');
			std::string pathToProject = ProjectManager::GetInstance()->getPath().string();
			std::string pathToEngineDirectory = pathToEditor.substr(0, pathToEditor.find("Tengine") + 7);
			cmakeFile <<
				R"(cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME )" + projectName + R"()

project(${PROJECT_NAME})

set(DIRS_SRC_MODULE 
	"*.h" "*.cpp"
	)
file(GLOB TARGET_SRC_MODULE ${DIRS_SRC_MODULE})

include_directories()" + pathToEngineDirectory + R"(/core)
include_directories()" + pathToEngineDirectory + R"(/core)
include_directories()" + pathToEngineDirectory + R"(/external/glfw/include)
include_directories()" + pathToEngineDirectory + R"(/external/Assimp/include)
include_directories()" + pathToEngineDirectory + R"(/external/spdlog/include)
include_directories()" + pathToEngineDirectory + R"(/external/nlohmann/include)
include_directories()" + pathToEngineDirectory + R"(/external/stbi)
include_directories()" + pathToEngineDirectory + R"(/external/glm)
include_directories(/)



add_executable(${PROJECT_NAME} ${TARGET_SRC_MODULE})

target_link_directories(${PROJECT_NAME} PRIVATE
    )" + pathToEditor + R"(
)

target_link_libraries(${PROJECT_NAME} PRIVATE TengineCore)


target_compile_options(${PROJECT_NAME} PRIVATE /wd4251)
)";
			cmakeFile.close();
			std::string cmakeCommand = "cmake -S " + pathToProject + "/build/Game" + " -B " + pathToProject + "/build/Game";
			std::system(cmakeCommand.c_str());
		}
	}

	void ProjectBuilder::BuildSolution()
	{
		
		std::string cmakeBuildCommand;
		if (m_buildConfiguration == BuildConfiguration::Debug)
		{
			cmakeBuildCommand = "cmake --build " + ProjectManager::GetInstance()->getPath().string() + "/build/Game --config Debug";
		}
		else if(m_buildConfiguration == BuildConfiguration::Release)
		{
			cmakeBuildCommand = "cmake --build " + ProjectManager::GetInstance()->getPath().string() + "/build/Game --config Release";
		}
		std::system(cmakeBuildCommand.c_str());
	}
	void ProjectBuilder::CollectFiles()
	{
		std::string pathToProject = ProjectManager::GetInstance()->getPath().string();
		std::string projectName = ProjectManager::GetInstance()->getName();

		//Copy executable file
		if (m_buildConfiguration == BuildConfiguration::Debug)
		{
			std::filesystem::copy(pathToProject + "/build/Game/Debug/" + projectName + ".exe", pathToProject + "/build/Game/bin/" + projectName + ".exe",
				std::filesystem::copy_options::overwrite_existing);
		}
		else if (m_buildConfiguration == BuildConfiguration::Release)
		{
			std::filesystem::copy(pathToProject + "/build/Game/Release/" + projectName + ".exe", pathToProject + "/build/Game/bin/" + projectName + ".exe",
				std::filesystem::copy_options::overwrite_existing);
		}
		//Copy all scenes;
		std::vector<std::filesystem::path> pathToScenes = ProjectManager::GetInstance()->getPathToScenes();
		for (const auto& scene : pathToScenes)
		{
			std::filesystem::copy(pathToProject + "/" + scene.string(), pathToProject + "/build/Game/bin/" + scene.string(),
				std::filesystem::copy_options::overwrite_existing);
		}
		
		//Copy Core Shared Library
		std::string pathToEditor = std::filesystem::current_path().string();
		std::replace(pathToEditor.begin(), pathToEditor.end(), '\\', '/');
		if (m_buildConfiguration == BuildConfiguration::Debug)
		{
			std::filesystem::copy(pathToEditor + "/TengineCored.dll", pathToProject + "/build/Game/bin/TengineCore.dll",
				std::filesystem::copy_options::overwrite_existing);
		}
		else if (m_buildConfiguration == BuildConfiguration::Release)
		{
			std::filesystem::copy(pathToEditor + "/TengineCore.dll", pathToProject + "/build/Game/bin/TengineCore.dll",
				std::filesystem::copy_options::overwrite_existing);
		}
		//Copy ScriptModule
		std::filesystem::copy(pathToProject + "/build/ScriptModule/scriptModule.dll", pathToProject + "/build/Game/bin/scriptModule.dll",
			std::filesystem::copy_options::overwrite_existing);
	}
}

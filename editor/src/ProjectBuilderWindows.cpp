#include "ProjectBuilderWindows.h"

#include<filesystem>
#include<fstream>
#include<Core/Logger.h>

#include"ProjectManager.h"
#include"ProjectManager.h"
#include"Scripts/ScriptCompiler.h"
namespace TengineEditor
{
	std::filesystem::path ProjectBuilderWindows::m_pathToBuildDirectory;

	void ProjectBuilderWindows::Build()
	{
		std::string pathToProject = ProjectManager::GetInstance()->getPath().string();
		std::filesystem::create_directory(pathToProject + "/build");
		std::filesystem::create_directory(pathToProject + "/build/Windows");
		m_pathToBuildDirectory = pathToProject + "/build/Windows";
		GenerateInitFiles();
		GenerateCMake();
		BuildSolution();
		CollectFiles();
	}

	void ProjectBuilderWindows::GenerateInitFiles()
	{
		std::vector<ScriptInfo> scriptInfo = ScriptCompiler::GetScriptInfo();
		//Source file
		std::string projectName = ProjectManager::GetInstance()->getName();

		std::ofstream gameHeaderFile(m_pathToBuildDirectory.string() + "/Game.h");
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
		else
		{
			Logger::Critical("ERROR::ProjectBuilder::Error creating game header file");
		}

		std::ofstream gameScriptSystemHeaderFile(m_pathToBuildDirectory.string() + "/GameScriptSystem.h");
		if (gameScriptSystemHeaderFile.is_open())
		{
			gameScriptSystemHeaderFile << R"(#pragma once

#include<Systems/ScriptSystem.h>

namespace Tengine
{
	class GameScriptSystem : public ScriptSystem
	{
	public:
		void init() final;
		void update() final;
		void destroy() final;
		std::shared_ptr<Component> addScript(std::shared_ptr<Object> object, std::string_view nameScript) final;
		std::vector<std::string> getScriptNames() final;


		static std::shared_ptr<GameScriptSystem> GetInstance();
	};
}
)";
			gameScriptSystemHeaderFile.close();
		}
		else
		{
			Logger::Critical("ERROR::ProjectBuilder::Error creating GameScriptSystem header file");
		}
		std::ofstream gameScriptSystemCppFile(m_pathToBuildDirectory.string() + "/GameScriptSystem.cpp");
		if (gameScriptSystemCppFile.is_open())
		{
			gameScriptSystemCppFile << R"(#include "GameScriptSystem.h"

)";
			for (const auto& info : scriptInfo)
			{
				gameScriptSystemCppFile << "#include\"" + info.name + ".h\"\n";
			}
			gameScriptSystemCppFile << R"(

namespace Tengine
{

	void GameScriptSystem::init()
	{
		std::shared_ptr<Scene> scene = SceneManager::GetCurrentScene();
		if (scene)
		{
			std::vector<std::shared_ptr<Script>> scripts = scene->getComponents<Script>();
			for (const auto& script : scripts)
			{
				script->start();
			}
		}
	}

	void GameScriptSystem::update()
	{
		std::shared_ptr<Scene> scene = SceneManager::GetCurrentScene();
		if (scene)
		{
			std::vector<std::shared_ptr<Script>> scripts = scene->getComponents<Script>();
			for (const auto& script : scripts)
			{
				script->update();
			}
		}
	}

	void GameScriptSystem::destroy()
	{
		
	}

	std::shared_ptr<Component> GameScriptSystem::addScript(std::shared_ptr<Object> object, std::string_view nameScript)
	{
)";
			for (size_t i = 0; i < scriptInfo.size(); i++)
			{
				if (i == 0)
				{
					gameScriptSystemCppFile << "\t\tif (nameScript == \"" + scriptInfo[i].name + "\")\n\t{";
					gameScriptSystemCppFile << "\t\tstd::shared_ptr<Component> script = std::make_shared<" + scriptInfo[i].name + ">();\n";
					gameScriptSystemCppFile << "\t\tobject->addComponent(script);\n";
					gameScriptSystemCppFile << "\t\treturn script;\n\t}\n";
				}
				else
				{
					gameScriptSystemCppFile << "\t\telse if (nameScript == " + scriptInfo[i].name + ")\n";
					gameScriptSystemCppFile << "\t\tstd::shared_ptr<Component> script = std::make_shared<" + scriptInfo[i].name + ">();\n";
					gameScriptSystemCppFile << "\t\tobject->addComponent(script);\n";
					gameScriptSystemCppFile << "\t\treturn script;\n\t}\n";
				}
			}
			gameScriptSystemCppFile << "\t\treturn nullptr;\n";
			gameScriptSystemCppFile << R"(
	}

	std::vector<std::string> GameScriptSystem::getScriptNames()
	{
)";

			std::string nameAllScripts;
			for (size_t i = 0; i < scriptInfo.size(); i++)
			{
				if (i == scriptInfo.size() - 1)
				{
					nameAllScripts += "\"" + scriptInfo[i].name + "\"";

				}
				else
				{
					nameAllScripts += "\"" + scriptInfo[i].name + "\", ";
				}
			}
			gameScriptSystemCppFile << "\treturn {" + nameAllScripts + R"(};
	}

	std::shared_ptr<GameScriptSystem> GameScriptSystem::GetInstance()
	{
		if (!ScriptSystem::GetInstance())
		{
			SetInstance(std::make_shared<GameScriptSystem>());
		}
		return std::dynamic_pointer_cast<GameScriptSystem>(ScriptSystem::GetInstance());
	}

)";
			gameScriptSystemCppFile << ScriptCompiler::GetMetaInfo();
			gameScriptSystemCppFile << "\n}";
			gameScriptSystemCppFile.close();
		}
		else
		{
			Logger::Critical("ERROR::ProjectBuilder::Error creating GameScriptSystem cpp file");
		}
		std::ofstream gameCppFile(m_pathToBuildDirectory.string() + "/Game.cpp");
		if (gameCppFile.is_open())
		{
			gameCppFile << R"(#include"Game.h"

#include<Systems/ScriptSystem.h>
#include<Scene/SceneManager.h>
#include<ECS/SystemManager.h>

#include"GameScriptSystem.h"

Game::Game(unsigned int width, unsigned int height, const std::string& title) :
    Application(width, height, title)
{

}

void Game::create()
{
	GameScriptSystem::GetInstance();
)";
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
			Logger::Critical("ERROR::ProjectBuilder::Error creating game cpp file");
		}
		std::ofstream mainFile(m_pathToBuildDirectory.string() + "/main.cpp");
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

	void ProjectBuilderWindows::GenerateCMake()
	{
		std::string projectName = ProjectManager::GetInstance()->getName();
		std::ofstream cmakeFile(m_pathToBuildDirectory.string() + "/CMakeLists.txt");
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
	"../../Scripts/*.h" "../../Scripts/*.cpp" 
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
include_directories(../../Scripts)
include_directories(/)



add_executable(${PROJECT_NAME} ${TARGET_SRC_MODULE})

target_link_directories(${PROJECT_NAME} PRIVATE
    )" + pathToEditor + R"(
)
)";
			if (ProjectBuilder::GetBuildConfiguration() == BuildConfiguration::Debug)
			{
				cmakeFile << "target_link_libraries(${PROJECT_NAME} PRIVATE TengineCored)\n";
			}
			else if (ProjectBuilder::GetBuildConfiguration() == BuildConfiguration::Release)
			{
				cmakeFile << "target_link_libraries(${PROJECT_NAME} PRIVATE TengineCore)\n";
			}
			cmakeFile.close();
			std::string cmakeCommand = "cmake -S " + m_pathToBuildDirectory.string() + " -B " + m_pathToBuildDirectory.string();
			std::system(cmakeCommand.c_str());
		}
	}

	void ProjectBuilderWindows::BuildSolution()
	{

		std::string cmakeBuildCommand;
		if (ProjectBuilder::GetBuildConfiguration() == BuildConfiguration::Debug)
		{
			cmakeBuildCommand = "cmake --build " + m_pathToBuildDirectory.string() + " --config Debug";
		}
		else if (ProjectBuilder::GetBuildConfiguration() == BuildConfiguration::Release)
		{
			cmakeBuildCommand = "cmake --build " + m_pathToBuildDirectory.string() + " --config Release";
		}
		std::system(cmakeBuildCommand.c_str());
	}

	void ProjectBuilderWindows::CollectFiles()
	{
		std::string projectName = ProjectManager::GetInstance()->getName();
		std::filesystem::remove_all(m_pathToBuildDirectory.string() + "/bin");
		std::filesystem::create_directory(m_pathToBuildDirectory.string() + "/bin");

		//Copy executable file
		if (ProjectBuilder::GetBuildConfiguration() == BuildConfiguration::Debug)
		{
			std::filesystem::copy(m_pathToBuildDirectory.string() + "/Debug/" + projectName + ".exe", m_pathToBuildDirectory.string() + "/bin/" + projectName + ".exe",
				std::filesystem::copy_options::overwrite_existing);
		}
		else if (ProjectBuilder::GetBuildConfiguration() == BuildConfiguration::Release)
		{
			std::filesystem::copy(m_pathToBuildDirectory.string() + "/Release/" + projectName + ".exe", m_pathToBuildDirectory.string() + "/bin/" + projectName + ".exe",
				std::filesystem::copy_options::overwrite_existing);
		}
		//Copy all scenes;
		std::vector<std::filesystem::path> pathToScenes = ProjectManager::GetInstance()->getPathToScenes();
		for (const auto& scene : pathToScenes)
		{
			std::filesystem::copy(ProjectManager::GetInstance()->getPath().string() + "/" + scene.string(), m_pathToBuildDirectory.string() + "/bin/" + scene.string(),
				std::filesystem::copy_options::overwrite_existing);
		}

		//Copy Core Shared Library
		std::string pathToEditor = std::filesystem::current_path().string();
		std::replace(pathToEditor.begin(), pathToEditor.end(), '\\', '/');
		if (ProjectBuilder::GetBuildConfiguration() == BuildConfiguration::Debug)
		{
			std::filesystem::copy(pathToEditor + "/TengineCored.dll", m_pathToBuildDirectory.string() + "/bin/TengineCored.dll",
				std::filesystem::copy_options::overwrite_existing);
		}
		else if (ProjectBuilder::GetBuildConfiguration() == BuildConfiguration::Release)
		{
			std::filesystem::copy(pathToEditor + "/TengineCore.dll", m_pathToBuildDirectory.string() + "/bin/TengineCore.dll",
				std::filesystem::copy_options::overwrite_existing);
		}
	}
}

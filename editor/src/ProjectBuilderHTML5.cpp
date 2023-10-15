#include "ProjectBuilderHTML5.h"

#include<filesystem>
#include<fstream>
#include<Core/Logger.h>

#include"ProjectManager.h"
#include"ProjectManager.h"
#include"Scripts/ScriptCompiler.h"

namespace TengineEditor
{
	std::filesystem::path ProjectBuilderHTML5::m_pathToBuildDirectory;

	void ProjectBuilderHTML5::Build()
	{
		std::string pathToProject = ProjectManager::GetInstance()->getPath().string();
		std::filesystem::create_directory(pathToProject + "/build");
		std::filesystem::create_directory(pathToProject + "/build/HTML5");
		m_pathToBuildDirectory = pathToProject + "/build/HTML5";
		GenerateInitFiles();
		GenerateCMake();
		BuildSolution();
		CollectFiles();
	}

	void ProjectBuilderHTML5::GenerateInitFiles()
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

#include<emscripten.h>

Game game = Game(1024, 512, "ExampleProject");

void UpdateGame()
{
    game.tick();
}

int main(int argc, char** argv)
{
    game.init();
    emscripten_set_main_loop(UpdateGame, 0, 1);

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

	void ProjectBuilderHTML5::GenerateCMake()
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
	
	")" + pathToEditor + R"(/src/core/Core/*.h"			
	")" + pathToEditor + R"(/src/core/ECS/*.h" 			
	")" + pathToEditor + R"(/src/core/Components/*.h" 			
	")" + pathToEditor + R"(/src/core/Systems/*.h" 			
	")" + pathToEditor + R"(/src/core/Renderer/*.h"		
	")" + pathToEditor + R"(/src/core/Renderer/OpenGL/*.h"
	")" + pathToEditor + R"(/src/core/Renderer/Shaders/*.h"	
	")" + pathToEditor + R"(/src/core/Renderer/Shaders/GLSL/*.h"	
	")" + pathToEditor + R"(/src/core/Scene/*.h" 		
	")" + pathToEditor + R"(/src/core/Utils/*.h"          
	")" + pathToEditor + R"(/src/core/Utils/EditorElements/*.h"
	")" + pathToEditor + R"(/src/core/Core/*.cpp"                
	")" + pathToEditor + R"(/src/core/ECS/*.cpp"                 
	")" + pathToEditor + R"(/src/core/Components/*.cpp"      	
	")" + pathToEditor + R"(/src/core/Systems/*.cpp"     		
	")" + pathToEditor + R"(/src/core/Renderer/*.cpp"            
	")" + pathToEditor + R"(/src/core/Renderer/OpenGL/*.cpp"
	")" + pathToEditor + R"(/src/core/Renderer/Shaders/*.cpp"	
	")" + pathToEditor + R"(/src/core/Renderer/Shaders/GLSL/*.cpp"          
	")" + pathToEditor + R"(/src/core/Scene/*.cpp"               
	")" + pathToEditor + R"(/src/core/Utils/*.cpp"               
	")" + pathToEditor + R"(/src/core/Utils/EditorElements/*.cpp"  
	)
file(GLOB TARGET_SRC_MODULE ${DIRS_SRC_MODULE})
set(CMAKE_EXECUTABLE_SUFFIX ".html")

add_executable(${PROJECT_NAME} ${TARGET_SRC_MODULE})

target_include_directories(${PROJECT_NAME} PUBLIC )" + pathToEditor + R"(/src/core)
target_include_directories(${PROJECT_NAME} PUBLIC )" + pathToEditor + R"(/src/external/glfw/include)
target_include_directories(${PROJECT_NAME} PUBLIC )" + pathToEditor + R"(/src/external/Assimp/include)
target_include_directories(${PROJECT_NAME} PUBLIC )" + pathToEditor + R"(/src/external/spdlog/include)
target_include_directories(${PROJECT_NAME} PUBLIC )" + pathToEditor + R"(/src/external/nlohmann/include)
target_include_directories(${PROJECT_NAME} PUBLIC )" + pathToEditor + R"(/src/external/stbi)
target_include_directories(${PROJECT_NAME} PUBLIC )" + pathToEditor + R"(/src/external/glm)
target_include_directories(${PROJECT_NAME} PUBLIC ../../Scripts)
target_include_directories(${PROJECT_NAME} PUBLIC /)

add_subdirectory()" + pathToEditor + R"(/src/external/glad ${CMAKE_CURRENT_BINARY_DIR}/bin/glad)
add_subdirectory()" + pathToEditor + R"(/src/external/spdlog ${CMAKE_CURRENT_BINARY_DIR}/bin/spdlog)
add_subdirectory()" + pathToEditor + R"(/src/external/glm ${CMAKE_CURRENT_BINARY_DIR}/bin/glm)
add_subdirectory()" + pathToEditor + R"(/src/external/stbi ${CMAKE_CURRENT_BINARY_DIR}/bin/stbi)
add_subdirectory()" + pathToEditor + R"(/src/external/Assimp ${CMAKE_CURRENT_BINARY_DIR}/bin/Assimp)
target_include_directories(${PROJECT_NAME} PUBLIC )" + pathToEditor + R"(/src/external/Assimp/include)
add_subdirectory()" + pathToEditor + R"(/src/external/nlohmann ${CMAKE_CURRENT_BINARY_DIR}/bin/nlohmann)

target_link_libraries(${PROJECT_NAME} PRIVATE glad spdlog glm stbi assimp nlohmann_json)

target_compile_options(spdlog PUBLIC "-pthread")
target_compile_options(stbi PUBLIC "-pthread")
target_compile_options(glad PUBLIC "-pthread")
target_compile_options(assimp PUBLIC "-pthread")
target_compile_options(${PROJECT_NAME} PUBLIC "-pthread")

target_link_options(${PROJECT_NAME} PUBLIC -std=c++17 -pthread --embed-file=data --embed-file=DefaultScene.scene -sUSE_WEBGL2=1 -sUSE_GLFW=3 -sWASM=1 -sDEMANGLE_SUPPORT=1 -sFULL_ES3=1 -sTOTAL_MEMORY=2gb -sALLOW_MEMORY_GROWTH=1 -sUSE_PTHREADS=1 -sSHARED_MEMORY=1 -sPTHREAD_POOL_SIZE=4 -sPTHREAD_POOL_SIZE_STRICT=0)
)";
			cmakeFile.close();

			std::string cmakeCommand;

			if (ProjectBuilder::GetBuildConfiguration() == BuildConfiguration::Debug)
			{
				cmakeCommand = "emcmake cmake -S " + m_pathToBuildDirectory.string() + " -B " + m_pathToBuildDirectory.string() + " -DCMAKE_BUILD_TYPE=DEBUG";
			}
			else if (ProjectBuilder::GetBuildConfiguration() == BuildConfiguration::Release)
			{
				cmakeCommand = "emcmake cmake -S " + m_pathToBuildDirectory.string() + " -B " + m_pathToBuildDirectory.string() + " -DCMAKE_BUILD_TYPE=RELEASE";
			}
			std::system(cmakeCommand.c_str());
		}
	}

	void ProjectBuilderHTML5::BuildSolution()
	{
		std::string buildCommand = "emmake make -C " + m_pathToBuildDirectory.string() + " -j 4";
		std::system(buildCommand.c_str());
	}

	void ProjectBuilderHTML5::CollectFiles()
	{

	}
}

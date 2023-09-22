#include "CodeGenerator.h"

#include<fstream>
#include<filesystem>

#include"Core/Logger.h"
#include"Systems/ScriptSystem.h"

namespace Tengine
{
    std::vector<ScriptInfo> CodeGenerator::m_scriptInfo;

    void CodeGenerator::CompileScripts()
    {
        ScriptSystem::GetInstance()->freeModule();
        std::filesystem::create_directory("Scripts");
        std::filesystem::create_directory("Scripts/build");
        FindAllScripts();
        GenerateInitFiles();
        GenerateCmake();
        BuildDll();
        ScriptSystem::GetInstance()->reload();
    }
    void CodeGenerator::BuildDll()
    {
        std::system("cmake --build Scripts/build --config Debug");
    }
    void CodeGenerator::GenerateInitFiles()
    {
        //Header file
        std::ofstream initHeaderFile("Scripts/build/SystemModule.h");
        if (initHeaderFile.is_open())
        {
            initHeaderFile << \
R"(#pragma once

#include<string>
#include<vector>

#include"ECS/Object.h"

using namespace Tengine;
#define EXTERN __declspec(dllexport)

extern "C" EXTERN void StartScripts();

extern "C" EXTERN void UpdateScripts();

extern "C" EXTERN void AddScript(std::shared_ptr<Object> object, std::string_view nameScript);

extern "C" EXTERN std::vector<std::string> GetScriptNames();
)";
            initHeaderFile.close();
        }
        else
        {
            Logger::Critical("ERROR::CodeGenerator::Error creating init header file");
        }

        //Source file
        std::ofstream initSourceFile("Scripts/build/SystemModule.cpp");
        if (initSourceFile.is_open())
        {
            initSourceFile << \
                R"(#include"SystemModule.h"

#include"Core/Logger.h"
#include"Scene/SceneManager.h"
#include"Components/Script.h"
)";
            for (const auto& info : m_scriptInfo)
            {
                initSourceFile << "#include\"" + info.name + ".h\"\n";
            }
            initSourceFile << R"(
using namespace Tengine;

void StartScripts()
{
    std::shared_ptr<Scene> scene = SceneManager::GetCurrentScene();
    if(scene)
    {
        std::vector<std::shared_ptr<Script>> scripts = scene->getComponents<Script>();
        for (const auto& script : scripts)
        {
            script->start();
        }
    }
}

void UpdateScripts()
{
    std::shared_ptr<Scene> scene = SceneManager::GetCurrentScene();
    if(scene)
    {
        std::vector<std::shared_ptr<Script>> scripts = scene->getComponents<Script>();
        for (const auto& script : scripts)
        {
            script->update();
        }
    }
}
std::vector<std::string> GetScriptNames()
{
)";
            std::string nameAllScripts;
            for (size_t i = 0; i < m_scriptInfo.size(); i++)
            {
                if (i == m_scriptInfo.size() - 1)
                {
                    nameAllScripts += "\"" + m_scriptInfo[i].name + "\"";

                }
                else
                {
                    nameAllScripts += "\"" +m_scriptInfo[i].name + "\", ";
                }
            }
            initSourceFile << "    return {" + nameAllScripts + R"(};
}
)";
            initSourceFile << R"(void AddScript(std::shared_ptr<Object> object, std::string_view nameScript)
{
)";
            for (size_t i = 0; i < m_scriptInfo.size(); i++)
            {
                if (i == 0)
                {
                    initSourceFile << R"(    if (nameScript == ")" + m_scriptInfo[i].name + R"(")
    {
)";
                    initSourceFile << R"(        object->addComponent(Component::Create<)" + m_scriptInfo[i].name + R"(>());
    }
)";
                }
                else
                {
                    initSourceFile << R"(    else if (nameScript == )" + m_scriptInfo[i].name + R"()
    {
)";
                    initSourceFile << R"(    object->addComponent(Component::Create<" + m_scriptInfo[i].name + R"(>());
    }
)";
                }
            }
            initSourceFile << "\n}";
            initSourceFile.close();
        }
        else
        {
            Logger::Critical("ERROR::CodeGenerator::Error creating init source file");
        }
    }
    void CodeGenerator::FindAllScripts()
    {
        m_scriptInfo.clear();
        for (const auto& entry : std::filesystem::directory_iterator("Scripts")) {
            if (entry.is_regular_file() && entry.path().extension() == ".h")
            {
                ScriptInfo info;
                info.path = entry.path();
                info.name = entry.path().stem().filename().string();
                m_scriptInfo.push_back(info);
            }
        }
    }
    void CodeGenerator::GenerateCmake()
	{
		std::ofstream cmakeFile("Scripts/build/CMakeLists.txt");
        if (cmakeFile.is_open()) {
            cmakeFile <<
R"(cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME ScriptModule)

project(${PROJECT_NAME})

set(DIRS_SRC_MODULE 
	"*.h" "*.cpp"
    "../*.h" "../*.cpp" 
	)
file(GLOB TARGET_SRC_MODULE ${DIRS_SRC_MODULE})

include_directories(../../../../core)
include_directories(../../../../external/spdlog/include)
include_directories(../../../../external/nlohmann/include)
include_directories(../../../../external/glm)
include_directories(..)



add_library(${PROJECT_NAME} SHARED ${TARGET_SRC_MODULE})

target_link_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/../..
)

target_link_libraries(${PROJECT_NAME} PRIVATE TengineCored)


target_compile_options(${PROJECT_NAME} PRIVATE /wd4251)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_BINARY_DIR}/Debug/ScriptModule.dll
        ${CMAKE_SOURCE_DIR}/scriptModule.dll
)

)";
            cmakeFile.close();
        }
        else {
            Logger::Critical("ERROR::CodeGenerator::Error creating Premake file");
        }
        std::system("cmake -S Scripts/build -B Scripts/build");
	}
}
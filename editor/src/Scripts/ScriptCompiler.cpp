#include "ScriptCompiler.h"

#include<fstream>
#include<filesystem>
#include<sstream>
#include<Core/Logger.h>
#include<Systems/ScriptSystem.h>
#include"ProjectManager.h"
#include"EditorScriptSystem.h"
#include"FileManager.h"

namespace TengineEditor
{
    using namespace Tengine;

    std::vector<std::string> splitString(const std::string& input) {
        std::vector<std::string> tokens;
        std::string token;
        std::string delimiter = " ;=,:(){}";

        for (char c : input) {
            if (c == '\t' || c == '\n')
            {
                continue;
            }
            if (delimiter.find(c) != std::string::npos) {
                if (!token.empty()) {
                    tokens.push_back(token);
                    token.clear();
                }
                if (c != ' ')
                {
                    tokens.push_back(std::string(1, c));
                }
            }
            else {
                token += c;
            }
        }

        if (!token.empty()) {
            tokens.push_back(token);
        }

        return tokens;
    }
    BuildConfiguration ScriptCompiler::m_scriptBuildConfiguration;
    BuildConfiguration ScriptCompiler::m_coreBuildConfiguration;

    void ScriptCompiler::Compile()
    {
        EditorScriptSystem::GetInstance()->freeModule();
        std::filesystem::create_directory("Scripts");
        std::filesystem::create_directory("builds");
        std::filesystem::create_directory("builds/ScriptModule");
        GenerateInitFiles();
        GenerateCmake();
        BuildDll();
        EditorScriptSystem::GetInstance()->reload();
    }
    void ScriptCompiler::SetScriptBuildConfiguration(BuildConfiguration config)
    {
        m_scriptBuildConfiguration = config;
    }
    void ScriptCompiler::SetCoreBuildConfiguration(BuildConfiguration config)
    {
        m_coreBuildConfiguration = config;
    }
    BuildConfiguration ScriptCompiler::GetScriptBuildConfiguration()
    {
        return m_scriptBuildConfiguration;
    }
    BuildConfiguration ScriptCompiler::GetCoreBuildConfiguration()
    {
        return m_coreBuildConfiguration;
    }
    std::vector<ScriptInfo> ScriptCompiler::GetScriptInfo()
    {
        std::vector<ScriptInfo> scriptinfo;
        for (const auto& entry : std::filesystem::directory_iterator("Scripts")) {
            if (entry.is_regular_file() && entry.path().extension() == ".h")
            {
                ScriptInfo info;
                info.path = entry.path();
                info.name = entry.path().stem().filename().string();
                std::ifstream file(info.path);
                std::string scriptCode;
                if (file.is_open())
                {
                    std::string line;
                    while (std::getline(file, line)) {
                        scriptCode += line + "\n";
                    }
                    scriptCode = scriptCode.substr(scriptCode.find("class " + info.name));
                    scriptCode = scriptCode.substr(scriptCode.find("{") + 1);
                    int counter = 1;
                    size_t lastPos;
                    for (size_t i = 0; i < scriptCode.length(); i++)
                    {
                        if (scriptCode[i] == '{')
                        {
                            counter++;
                        }
                        else if (scriptCode[i] == '}')
                        {
                            counter--;
                            if (counter <= 0)
                            {
                                lastPos = i;
                                break;
                            }
                        }

                    }
                    scriptCode = scriptCode.substr(0, lastPos);
                    std::vector<std::string> tokens = splitString(scriptCode);

                    bool inScope = false;
                    for (size_t i = 0; i < tokens.size(); i++)
                    {
                        if (tokens[i] == "{")
                        {
                            inScope = true;
                            continue;
                        }
                        else if (tokens[i] == "}")
                        {
                            inScope = false;
                            continue;
                        }
                        if (!inScope)
                        {
                            if (tokens[i] == "float" || tokens[i] == "int" || tokens[i] == "double")
                            {
                                std::string type = tokens[i];
                                i++;
                                if (tokens[i + 1] != "(")
                                {
                                    while (tokens[i] != ";")
                                    {
                                        if (tokens[i] == ",")
                                        {
                                            i++;
                                            continue;
                                        }
                                        if (tokens[i] == "=")
                                        {
                                            while (!(tokens[i] == ";" || tokens[i] == ","))
                                            {
                                                i++;
                                            }
                                            continue;
                                        }
                                        info.fields.push_back({ type,tokens[i] });
                                        i++;
                                    }
                                }
                            }
                        }
                    }
                }

                scriptinfo.push_back(info);
            }
        }
        return scriptinfo;
    }

    std::string ScriptCompiler::GetMetaInfo()
    {
        std::string metaData;
        for (const auto& info : GetScriptInfo())
        {
            metaData += "ComponentInfo " + info.name + R"(::getInfo()
{
    ComponentInfo displayInfo = ComponentInfo();
)";
            metaData += "\tdisplayInfo.setComponentName(\"" + info.name + "\");\n";
            int fieldCounter = 0;
            for (const auto& field : info.fields)
            {
                if (field.type == "float" || field.type == "double" || field.type == "int")
                {
                    std::string varName = "field" + std::to_string(fieldCounter);
                    metaData += "\tstd::shared_ptr<FieldFloat> " + varName + " = std::make_shared<FieldFloat>(); \n";
                    metaData += "\t" + varName + "->minValue = 0.0f;\n";
                    metaData += "\t" + varName + "->maxValue = 20.0f;\n";
                    metaData += "\t" + varName + "->name = \"" + field.name + "\";\n";
                    metaData += "\t" + varName + "->data = &" + field.name + ";\n";
                    metaData += "\tdisplayInfo.addElement(" + varName + ");\n";
                    fieldCounter++;
                }
            }

            metaData += "\treturn displayInfo;\n}\n";
        }
        return metaData;
    }

    void ScriptCompiler::GenerateInitFiles()
    {
        std::vector<ScriptInfo> scriptInfo = GetScriptInfo();

        //Header file
        std::ofstream initHeaderFile("builds/ScriptModule/SystemModule.h");
        if (initHeaderFile.is_open())
        {
            initHeaderFile << \
R"(#pragma once

#include<string>
#include<vector>
#include<memory>

#include"ECS/Object.h"

using namespace Tengine;
#define EXTERN __declspec(dllexport)

extern "C" EXTERN void StartScripts();

extern "C" EXTERN void UpdateScripts();

extern "C" EXTERN void* AddScript(std::string_view nameScript);

extern "C" EXTERN std::vector<std::string> GetScriptNames();
)";
            initHeaderFile.close();
        }
        else
        {
            Logger::Critical("ERROR::ScriptCompiler::Error creating init header file");
        }

        //Source file
        std::ofstream initSourceFile("builds/ScriptModule/SystemModule.cpp");
        if (initSourceFile.is_open())
        {
            initSourceFile << \
                R"(#include"SystemModule.h"

#include"Core/Logger.h"
#include"Scene/SceneManager.h"
#include"Components/Script.h"
)";
            for (const auto& info : scriptInfo)
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
            initSourceFile << "    return {" + nameAllScripts + R"(};
}
)";
            initSourceFile << R"(void* AddScript(std::string_view nameScript)
{
)";
            for (size_t i = 0; i < scriptInfo.size(); i++)
            {
                if (i == 0)
                {
                    initSourceFile << R"(    if (nameScript == ")" + scriptInfo[i].name + R"(")
    {
)";
                    initSourceFile <<"\t\treturn new " + scriptInfo[i].name + "(); \n\t }\n";
                }
                else
                {
                    initSourceFile << R"(    else if (nameScript == )" + scriptInfo[i].name + R"()
    {
)";
                    initSourceFile << "\t\treturn new " + scriptInfo[i].name + "(); \n\t }\n";
                }
            }

            initSourceFile << "\t\treturn nullptr;\n}\n";
            initSourceFile << GetMetaInfo();
            initSourceFile.close();
        }
        else
        {
            Logger::Critical("ERROR::ScriptCompiler::Error creating init source file");
        }
    }
    
    void ScriptCompiler::GenerateCmake()
	{
		std::ofstream cmakeFile("builds/ScriptModule/CMakeLists.txt");
        if (cmakeFile.is_open()) 
        {
            std::string pathToEditor = FileManager::GetPathToEditor().string();
            std::string pathToProject = ProjectManager::GetInstance()->getPath().string();
            std::string pathToEngineDirectory = pathToEditor.substr(0, pathToEditor.find("Tengine") + 7);
            std::string compilerOptions;
            std::string nameLib;
            if (m_coreBuildConfiguration == BuildConfiguration::Debug)
            {
                compilerOptions = R"(
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MDd")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MDd")
)";
                nameLib = "TengineCored";
            }
            else if (m_coreBuildConfiguration == BuildConfiguration::Release)
            {
                compilerOptions = R"(
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MT")
)";
                nameLib = "TengineCore";
            }
            cmakeFile <<
R"(cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
)" + compilerOptions + R"(

set(PROJECT_NAME ScriptModule)

project(${PROJECT_NAME})

set(DIRS_SRC_MODULE 
	"*.h" "*.cpp"
    "../../Scripts/*.h" "../../Scripts/*.cpp" 
	)
file(GLOB TARGET_SRC_MODULE ${DIRS_SRC_MODULE})

include_directories()" + pathToEngineDirectory+ R"(/core)
include_directories()" + pathToEngineDirectory+ R"(/external/glfw/include)
include_directories()" + pathToEngineDirectory+ R"(/external/Assimp/include)
include_directories()" + pathToEngineDirectory+ R"(/external/spdlog/include)
include_directories()" + pathToEngineDirectory+ R"(/external/nlohmann/include)
include_directories()" + pathToEngineDirectory+ R"(/external/stbi)
include_directories()" + pathToEngineDirectory+ R"(/external/glm)
include_directories(../../Scripts)
include_directories(/)

add_library(${PROJECT_NAME} SHARED ${TARGET_SRC_MODULE})

target_link_directories(${PROJECT_NAME} PRIVATE
    )" + pathToEditor + R"(
)

target_link_libraries(${PROJECT_NAME} PRIVATE )"+ nameLib + R"()


target_compile_options(${PROJECT_NAME} PRIVATE /wd4251)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_SOURCE_DIR}/$<TARGET_FILE_NAME:${PROJECT_NAME}>
)
)";
            cmakeFile.close();
            std::string cmakeCommand = "cmake -S builds/ScriptModule -B builds/ScriptModule";
            std::system(cmakeCommand.c_str());
        }
        else {
            Logger::Critical("ERROR::ScriptCompiler::Error creating CMakefile");
        }
	}

    void ScriptCompiler::BuildDll()
    {
        std::string cmakeBuildCommand;
        if (m_scriptBuildConfiguration == BuildConfiguration::Debug)
        {
            cmakeBuildCommand = "cmake --build builds/ScriptModule --config Debug";
        }
        else if (m_scriptBuildConfiguration == BuildConfiguration::Release)
        {
            cmakeBuildCommand = "cmake --build builds/ScriptModule --config Release";
        }
        std::system(cmakeBuildCommand.c_str());
    }
}
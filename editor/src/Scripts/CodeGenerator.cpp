#include "CodeGenerator.h"

#include<fstream>
#include<filesystem>
#include<sstream>
#include"Core/Logger.h"
#include"Systems/ScriptSystem.h"

namespace Tengine
{
    std::vector<std::string> splitString(const std::string& input) {
        std::vector<std::string> tokens;
        std::string token;
        std::string delimiter = " ;=,:()";

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

    std::vector<ScriptInfo> CodeGenerator::m_scriptInfo;
    std::string CodeGenerator::m_metaData;

    void CodeGenerator::CompileScripts()
    {
        ScriptSystem::GetInstance()->freeModule();
        std::filesystem::create_directory("Scripts");
        std::filesystem::create_directory("build");
        GetScriptInfo();
        GenerateMetaData();
        GenerateInitFiles();
        GenerateCmake();
        BuildDll();
        ScriptSystem::GetInstance()->reload();
    }
    void CodeGenerator::BuildDll()
    {
        std::system("cmake --build build --config Debug");
    }

    void CodeGenerator::GenerateInitFiles()
    {
        //Header file
        std::ofstream initHeaderFile("build/SystemModule.h");
        if (initHeaderFile.is_open())
        {
            initHeaderFile << \
R"(#pragma once

#include<string>
#include<vector>

#include"ECS/Object.h"

using namespace Tengine;
#define EXTERN __declspec(dllexport)
#define SHOWINEDITOR DisplayInfo getDisplayInfo() override;

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
        std::ofstream initSourceFile("build/SystemModule.cpp");
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
            initSourceFile << m_metaData;
            initSourceFile.close();
        }
        else
        {
            Logger::Critical("ERROR::CodeGenerator::Error creating init source file");
        }
    }

    void CodeGenerator::GetScriptInfo()
    {
        m_scriptInfo.clear();
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

                    for (size_t i = 0; i < tokens.size(); i++)
                    {
                        if (tokens[i] == "float" || tokens[i] == "int" || tokens[i] == "double")
                        {
                            std::string type = tokens[i];
                            i++;
                            if (tokens[i+1] != "(")
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
                
                m_scriptInfo.push_back(info);
            }
        }
    }

    void CodeGenerator::GenerateMetaData()
    {
        for (const auto& info : m_scriptInfo)
        {
            m_metaData += "DisplayInfo " + info.name + R"(::getDisplayInfo()
{
    DisplayInfo displayInfo;
)";
            m_metaData += "\tdisplayInfo.setComponentName(\"" + info.name + "\");\n";
            int sliderCounter = 0;
            for (const auto& field : info.fields)
            {
                if (field.type == "float" || field.type == "double" || field.type == "int")
                {
                    std::string varName = "slider" + std::to_string(sliderCounter);
                    m_metaData += "\tstd::shared_ptr<DisplayInfoElementSlider> " + varName + " = std::make_shared<DisplayInfoElementSlider>(); \n";
                    m_metaData += "\t" + varName + "->minValue = 0.0f;\n";
                    m_metaData += "\t" + varName + "->maxValue = 20.0f;\n";
                    m_metaData += "\t" + varName + "->name = \"" + field.name + "\";\n";
                    m_metaData += "\t" + varName + "->data = &" + field.name + ";\n";
                    m_metaData += "\tdisplayInfo.addElement(" + varName + ");\n";
                    sliderCounter++;
                }
            }

            m_metaData += "\treturn displayInfo;\n}\n";
        }
    }
    
    void CodeGenerator::GenerateCmake()
	{
		std::ofstream cmakeFile("build/CMakeLists.txt");
        if (cmakeFile.is_open()) {
            cmakeFile <<
R"(cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME ScriptModule)

project(${PROJECT_NAME})

set(DIRS_SRC_MODULE 
	"*.h" "*.cpp"
    "../Scripts/*.h" "../Scripts/*.cpp" 
	)
file(GLOB TARGET_SRC_MODULE ${DIRS_SRC_MODULE})

include_directories(../../../core)
include_directories(../../../external/spdlog/include)
include_directories(../../../external/nlohmann/include)
include_directories(../../../external/glm)
include_directories(../Scripts)
include_directories(/)



add_library(${PROJECT_NAME} SHARED ${TARGET_SRC_MODULE})

target_link_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/..
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
        std::system("cmake -S build -B build");
	}
}
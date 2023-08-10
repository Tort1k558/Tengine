#include "CodeGenerator.h"

#include<fstream>
#include<filesystem>

#include"Core/Logger.h"
#include"ECS/Systems/ScriptSystem.h"

namespace Tengine
{
    void CodeGenerator::UpdateScripts()
    {
        std::filesystem::create_directory("Scripts");
        GenerateInitFiles();
        GeneratePremake();
        System::GetInstance<ScriptSystem>()->freeModule();
        BuildDll();
        System::GetInstance<ScriptSystem>()->reload();
    }
    void CodeGenerator::BuildDll()
    {
        std::system("cmake --build Scripts --config Release");
    }
    void CodeGenerator::GenerateInitFiles()
    {
        //Header file
        std::ofstream initHeaderFile("Scripts/SystemModule.h");
        if (initHeaderFile.is_open())
        {
            initHeaderFile << \
R"(#pragma once

#define EXTERN __declspec(dllexport)

extern "C" EXTERN void StartScripts();

extern "C" EXTERN void UpdateScripts();
)";
            initHeaderFile.close();
        }
        else
        {
            Logger::Critical("ERROR::CodeGenerator::Error creating init header file");
        }

        //Source file
        std::ofstream initSourceFile("Scripts/SystemModule.cpp");
        if (initSourceFile.is_open())
        {
            initSourceFile << \
                R"(#include"SystemModule.h"

#include"Core/Logger.h"

void StartScripts()
{
    Tengine::Logger::Info("Hello from ScriptModule: Start Function");
}

void UpdateScripts()
{
    Tengine::Logger::Info("Hello from ScriptModule: Start Function");
}
)";
            initSourceFile.close();
        }
        else
        {
            Logger::Critical("ERROR::CodeGenerator::Error creating init source file");
        }
    }
    void CodeGenerator::GeneratePremake()
	{
		std::ofstream cmakeFile("Scripts/CMakeLists.txt");
        if (cmakeFile.is_open()) {
            cmakeFile <<
R"(cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME ScriptModule)

project(${PROJECT_NAME})

set(DIRS_SRC_MODULE 
	"*.h" "*.cpp" 
	)
file(GLOB TARGET_SRC_MODULE ${DIRS_SRC_MODULE})

include_directories(../../../core)
include_directories(../../../external/spdlog/include)


add_library(${PROJECT_NAME} SHARED ${TARGET_SRC_MODULE})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_BINARY_DIR}/Release/ScriptModule.dll
        ${CMAKE_SOURCE_DIR}/scriptModule.dll
)

)";
            cmakeFile.close();
        }
        else {
            Logger::Critical("ERROR::CodeGenerator::Error creating Premake file");
        }
        std::system("cmake -S Scripts -B Scripts");
	}
}
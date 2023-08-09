#include "CodeGenerator.h"

#include<fstream>
#include<filesystem>

#include"Core/Logger.h"
#include"ECS/Systems/ScriptSystem.h"

namespace Tengine
{
    void CodeGenerator::UpdateScripts()
    {
        GenerateInitFiles();
        GeneratePremake();
        System::GetInstance<ScriptSystem>()->reload();
        BuildPremake();
    }
    void CodeGenerator::BuildPremake()
    {

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
		std::ofstream premakeFile("premakeScriptModule.lua");
        if (premakeFile.is_open()) {
            premakeFile <<
R"(workspace "Script"
    configurations { "Debug", "Release" }
    platforms { "x64" }

project "ScriptModule"
    kind "SharedLib"
    language "C++"
    targetdir ""
    files { "scripts/**.cpp" , "scripts/**.h" }
    includedirs { 
        "../../Core",
        "../../external/spdlog/include" 
}
)";
            premakeFile.close();
        }
        else {
            Logger::Critical("ERROR::CodeGenerator::Error creating Premake file");
        }
        std::system("premake5 --file=premakeScriptModule.lua vs2022");
	}
}
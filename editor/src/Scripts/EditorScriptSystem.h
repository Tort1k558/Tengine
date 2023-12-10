#pragma once

#include<Windows.h>
#include<string>

#include<Scene/System.h>
#include<Scene/SceneManager.h>
#include<Core/Core.h>
#include<Scene/Object.h>
#include<Components/Script.h>
#include<Systems/ScriptSystem.h>

namespace Tengine
{
	class EditorScriptSystem : public ScriptSystem
	{
		using UpdateScriptsFn = void(*)();
		using StartScriptsFn = void(*)();
		using AddScriptFn = void*(*)(std::string_view nameScript);
		using GetScriptNamesFn = std::vector<std::string>(*)();
	public:
		void init() final;
		void update() final;
		void destroy() final;
		void reload();
		void freeModule();
		std::shared_ptr<Component> addScript(std::shared_ptr<Object> object, std::string_view nameScript);
		void setPathToDll(std::filesystem::path path);
		std::filesystem::path getPathToDll();
		std::vector<std::string> getScriptNames();


		static std::shared_ptr<EditorScriptSystem> GetInstance();
	private:
		FARPROC __stdcall tryLoadFunction(HMODULE module, std::string_view functionName);
		StartScriptsFn m_startScripts = nullptr;
		UpdateScriptsFn m_updateScripts = nullptr;
		AddScriptFn m_addScript = nullptr;
		GetScriptNamesFn m_getScriptNames = nullptr;
		HMODULE m_dllHandle = nullptr;
		bool m_isLoaded = false;
		std::filesystem::path m_pathToDll;
	};
}
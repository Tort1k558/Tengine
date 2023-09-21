#pragma once

#include<Windows.h>
#include<string>

#include"ECS/System.h"
#include"Scene/SceneManager.h"
#include"Core/Core.h"
#include"ECS/Object.h"

namespace Tengine
{
	class TENGINE ScriptSystem : public System
	{
		using UpdateScriptsFn = void(*)();
		using StartScriptsFn = void(*)();
		using AddScriptFn = void(*)(std::shared_ptr<Object> object, std::string_view nameScript);
		using GetScriptNamesFn = std::vector<std::string>(*)();
	public:
		void init() final;
		void update() final;
		void destroy() final;
		void reload();
		void freeModule();
		void addScript(std::shared_ptr<Object> object, std::string_view nameScript);
		std::vector<std::string> getScriptNames();
		static std::shared_ptr<ScriptSystem> GetInstance();
	private:
		FARPROC __stdcall tryLoadFunction(HMODULE module, std::string_view functionName);
		StartScriptsFn m_startScripts = nullptr;
		UpdateScriptsFn m_updateScripts = nullptr;
		AddScriptFn m_addScript = nullptr;
		GetScriptNamesFn m_getScriptNames = nullptr;
		HMODULE m_dllHandle = nullptr;
		bool m_isLoaded = false;

		static std::shared_ptr<ScriptSystem> m_instance;
	};
}
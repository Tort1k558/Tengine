#pragma once

#include<Windows.h>
#include<string>

#include"ECS/System.h"
#include"Scene/SceneManager.h"
#include"Core/Core.h"

namespace Tengine
{
	class TENGINE ScriptSystem : public System
	{
		using UpdateScriptsFn = void(*)();
		using StartScriptsFn = void(*)();
	public:
		void init() final;
		void update() final;
		void destroy() final;
		void reload();
		void freeModule();
	private:
		FARPROC __stdcall tryLoadFunction(HMODULE module, std::string_view functionName);
		StartScriptsFn m_startScripts = nullptr;
		UpdateScriptsFn m_updateScripts = nullptr;
		HMODULE m_dllHandle = nullptr;
		bool m_isLoaded = false;
	};
}
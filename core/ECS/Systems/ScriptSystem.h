#pragma once

#include<Windows.h>
#include<string>

#include"ECS/System.h"

namespace Tengine
{
	class ScriptSystem : public System
	{
		using UpdateScriptsFn = void(*)();
		using StartScriptsFn = void(*)();
	public:
		void init() final;
		void update() final;
		void destroy() final;
		void reload();
	private:
		FARPROC __stdcall tryLoadFunction(HMODULE module, std::string_view functionName);
		StartScriptsFn m_startScripts = nullptr;
		UpdateScriptsFn m_updateScripts = nullptr;
		HMODULE m_dllHandle = nullptr;
	};
}
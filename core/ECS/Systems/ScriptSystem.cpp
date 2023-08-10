#include "ScriptSystem.h"

#include"Core/Logger.h"

namespace Tengine
{
	void ScriptSystem::init()
	{
		reload();
		if (m_startScripts)
		{
			m_startScripts();
		}
	}

	void ScriptSystem::update()
	{
		if (m_updateScripts)
		{
			m_updateScripts();
		}
	}

	void ScriptSystem::destroy()
	{
		freeModule();
	}
	
	void ScriptSystem::reload()
	{
		m_dllHandle = LoadLibrary("Scripts/scriptModule.dll");
		if (m_dllHandle == nullptr) {
			Logger::Critical("ERROR::ScriptSystem::Could not load script dll: scriptModule.dll");
			return;
		}
		m_startScripts = (StartScriptsFn)tryLoadFunction(m_dllHandle, "StartScripts");
		m_updateScripts = (UpdateScriptsFn)tryLoadFunction(m_dllHandle, "UpdateScripts");
		m_isLoaded = true;
	}

	void ScriptSystem::freeModule()
	{
		if (m_isLoaded)
		{
			m_startScripts = nullptr;
			m_updateScripts = nullptr;

			if (!FreeLibrary(m_dllHandle))
			{
				DWORD errorCode = GetLastError();
				Logger::Critical("ERROR::ScriptSystem::Could not free script dll: {0}", errorCode);
				return;
			}
			m_isLoaded = false;
		}
	}

	FARPROC __stdcall ScriptSystem::tryLoadFunction(HMODULE module, std::string_view functionName)
	{
		auto func = GetProcAddress(module, functionName.data());
		if (func == nullptr)
		{
			Logger::Critical("ERROR::ScriptSystem::Could not load dll function: {0}", functionName);
		}

		return func;
	}
}
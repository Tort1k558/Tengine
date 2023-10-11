#include"EditorScriptSystem.h"

#include"Core/Logger.h"

namespace Tengine
{
	void EditorScriptSystem::init()
	{
		reload();
		if (m_startScripts)
		{
			m_startScripts();
		}
	}

	void EditorScriptSystem::update()
	{
		if (m_updateScripts)
		{
			m_updateScripts();
		}
	}

	void FreeScriptModule()
	{
		EditorScriptSystem::GetInstance()->freeModule();
	}

	void EditorScriptSystem::destroy()
	{
		std::atexit(FreeScriptModule);
	}
	void EditorScriptSystem::reload()
	{
		m_dllHandle = LoadLibrary(m_pathToDll.string().c_str());
		if (m_dllHandle == nullptr) {
			Logger::Critical("ERROR::ScriptSystem::Could not load script dll: scriptModule.dll");
			return;
		}
		m_startScripts = (StartScriptsFn)tryLoadFunction(m_dllHandle, "StartScripts");
		m_updateScripts = (UpdateScriptsFn)tryLoadFunction(m_dllHandle, "UpdateScripts");
		m_addScript = (AddScriptFn)tryLoadFunction(m_dllHandle, "AddScript");
		m_getScriptNames = (GetScriptNamesFn)tryLoadFunction(m_dllHandle, "GetScriptNames");
		m_isLoaded = true;
	}

	void EditorScriptSystem::freeModule()
	{
		if (m_isLoaded)
		{
			m_startScripts = nullptr;
			m_updateScripts = nullptr;
			m_addScript = nullptr;
			m_getScriptNames = nullptr;

			if (!FreeLibrary(m_dllHandle))
			{
				DWORD errorCode = GetLastError();
				Logger::Critical("ERROR::ScriptSystem::Could not free script dll: {0}", errorCode);
				return;
			}
			m_isLoaded = false;
		}
	}

	std::shared_ptr<Component> EditorScriptSystem::addScript(std::shared_ptr<Object> object, std::string_view nameScript)
	{
		if (m_addScript)
		{
			void* script = m_addScript(nameScript);
			if (script)
			{
				std::shared_ptr<Component> component(static_cast<Component*>(script));
				object->addComponent(component);
				return component;
			}
		}
		return nullptr;
	}

	void EditorScriptSystem::setPathToDll(std::filesystem::path path)
	{
		m_pathToDll = path;
	}

	std::filesystem::path EditorScriptSystem::getPathToDll()
	{
		return m_pathToDll;
	}

	std::vector<std::string> EditorScriptSystem::getScriptNames()
	{
		if (m_getScriptNames)
		{
			return m_getScriptNames();
		}
		return {};
	}

	std::shared_ptr<EditorScriptSystem> EditorScriptSystem::GetInstance()
	{
		if (!ScriptSystem::GetInstance())
		{
			SetInstance(std::make_shared<EditorScriptSystem>());
		}
		return std::dynamic_pointer_cast<EditorScriptSystem>(ScriptSystem::GetInstance());
	}

	FARPROC __stdcall EditorScriptSystem::tryLoadFunction(HMODULE module, std::string_view functionName)
	{
		auto func = GetProcAddress(module, functionName.data());
		if (func == nullptr)
		{
			Logger::Critical("ERROR::ScriptSystem::Could not load dll function: {0}", functionName);
		}

		return func;
	}
}
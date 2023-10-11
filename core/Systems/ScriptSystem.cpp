#include "ScriptSystem.h"

#include"Core/Logger.h"

namespace Tengine
{
	std::shared_ptr<ScriptSystem> ScriptSystem::m_instance;

	void ScriptSystem::SetInstance(std::shared_ptr<ScriptSystem> system)
	{
		m_instance = system;
	}
	std::shared_ptr<ScriptSystem> ScriptSystem::GetInstance()
	{
		if (!m_instance)
		{
			return nullptr;
		}
		return m_instance;
	}
}

#include "SystemManager.h"

#include<thread>

namespace Tengine
{
	std::vector<std::shared_ptr<System>> SystemManager::m_systems;

	void SystemManager::AddSystem(std::shared_ptr<System> system)
	{
		for (const auto& sys : m_systems)
		{
			if (sys.get() == system.get())
			{
				return;
			}
		}
		m_systems.push_back(system);
	}

	void SystemManager::RemoveSystem(std::shared_ptr<System> system)
	{
		for (size_t i = 0; i < m_systems.size(); i++)
		{
			if (system.get() == m_systems[i].get())
			{
				m_systems[i]->destroy();
				m_systems.erase(m_systems.begin() + i);
			}
		}
	}

	void SystemManager::InitSystems()
	{
		for (auto& system : m_systems)
		{
			if (system->isInitialized())
			{
				continue;
			}
			system->init();
			system->m_isInitialized = true;
		}
	}

	void SystemManager::UpdateSystems()
	{
		for (auto& system : m_systems)
		{
			system->update();
		}
	}

	void SystemManager::DestroySystems()
	{
		for (int i = m_systems.size() - 1; i >= 0; i--)
		{
			m_systems[i]->destroy();
			m_systems.erase(m_systems.begin() + i);
		}
	}
}
#include "SystemManager.h"

namespace Tengine
{
	std::vector<std::shared_ptr<System>> SystemManager::m_systems;

	void SystemManager::AddSystem(std::shared_ptr<System> system)
	{
		m_systems.push_back(system);
	}

	void SystemManager::RemoveSystem(std::shared_ptr<System> system)
	{
		//TODO
	}

	void SystemManager::InitSystems()
	{
		for (auto& system : m_systems)
		{
			system->init();
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
		for (auto& system : m_systems)
		{
			system->destroy();
		}
	}
}
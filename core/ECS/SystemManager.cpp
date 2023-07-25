#include "SystemManager.h"

std::unordered_map<size_t, std::shared_ptr<System>> SystemManager::m_systems;

void SystemManager::InitSystems()
{
	for (auto& system : m_systems)
	{
		system.second->init();
	}
}

void SystemManager::UpdateSystems()
{
	for (auto& system : m_systems)
	{
		system.second->update();
	}
}

void SystemManager::DestroySystems()
{
	for (auto& system : m_systems)
	{
		system.second->destroy();
	}
}
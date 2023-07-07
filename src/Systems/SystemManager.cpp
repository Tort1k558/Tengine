#include "SystemManager.h"

std::unordered_map<size_t, std::function<void()>> SystemManager::m_systemUpdateCalls = {};
std::unordered_map<size_t, std::function<void()>> SystemManager::m_systemInitCalls = {};

void SystemManager::InitSystems()
{
	for (const auto& call : m_systemInitCalls)
	{
		call.second();
	}
}

void SystemManager::UpdateSystems()
{
	for (const auto& call : m_systemUpdateCalls)
	{
		call.second();
	}
}

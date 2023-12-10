#include "SystemManager.h"

#include<Core/Timer.h>

namespace Tengine
{
	std::vector<SystemManager::SystemInfo> SystemManager::m_systemsInfo;

	void SystemManager::AddSystem(std::shared_ptr<System> system)
	{
		for (const auto& systemInfo : m_systemsInfo)
		{
			if (systemInfo.system.get() == system.get())
			{
				return;
			}
		}
		m_systemsInfo.push_back({ system });
	}

	void SystemManager::RemoveSystem(std::shared_ptr<System> system)
	{
		for (size_t i = 0; i < m_systemsInfo.size(); i++)
		{
			if (system.get() == m_systemsInfo[i].system.get())
			{
				m_systemsInfo[i].system->destroy();
				m_systemsInfo.erase(m_systemsInfo.begin() + i);
			}
		}
	}

	void SystemManager::InitSystems()
	{
		for (auto& systemInfo : m_systemsInfo)
		{
			if (systemInfo.system->isInitialized())
			{
				continue;
			}
			systemInfo.system->init();
			systemInfo.system->m_isInitialized = true;
		}
	}

	void SystemManager::UpdateSystems()
	{
		for (auto& systemInfo : m_systemsInfo)
		{
			if (!systemInfo.updateThread.joinable())
			{
				systemInfo.updateThread = std::thread([&systemInfo]()
					{
						while (!systemInfo.isDestroyed)
						{
							static std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<double>> nextFrame = Timer::GetNowPoint();
							if (systemInfo.system->getUpdatesPerSecond() != 0)
							{
								double maxDelta = 1.0 / static_cast<double>(systemInfo.system->getUpdatesPerSecond());

								if (Timer::GetDeltaTime() < maxDelta)
								{
									std::this_thread::sleep_until(nextFrame);
								}

								systemInfo.system->update();

								nextFrame += std::chrono::duration<double>(maxDelta);
							}
							else
							{
								std::this_thread::sleep_until(nextFrame);
								systemInfo.system->update();
								nextFrame += std::chrono::duration<double>(Timer::GetDeltaTime());
							}
						}
					});
			}
		}
	}

	void SystemManager::DestroySystems()
	{
		for (auto& systemInfo : m_systemsInfo)
		{
			systemInfo.isDestroyed = true;
			systemInfo.updateThread.join();
		}
		for (int i = m_systemsInfo.size() - 1; i >= 0; i--)
		{
			m_systemsInfo[i].system->destroy();
			m_systemsInfo.erase(m_systemsInfo.begin() + i);
		}
	}
}
#pragma once

#include<functional>
#include<unordered_map>
#include<thread>
#include"System.h"
namespace Tengine
{
	class TENGINE SystemManager
	{
	public:
		SystemManager() = delete;
		SystemManager(const SystemManager&) = delete;
		SystemManager(SystemManager&&) = delete;
		SystemManager& operator=(const SystemManager&) = delete;
		SystemManager& operator=(SystemManager&&) = delete;

		static void AddSystem(std::shared_ptr<System> system);
		static void RemoveSystem(std::shared_ptr<System> system);

		static void InitSystems();
		static void UpdateSystems();
		static void DestroySystems();
	private:
		struct SystemInfo
		{
			std::shared_ptr<System> system;
			std::thread updateThread;
			bool isDestroyed;
		};
		static std::vector<SystemInfo> m_systemsInfo;
	};
}
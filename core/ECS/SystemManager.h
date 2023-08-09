#pragma once

#include<functional>
#include<unordered_map>
#include"System.h"

namespace Tengine
{
	class SystemManager
	{
	public:
		SystemManager() = delete;
		SystemManager(const SystemManager&) = delete;
		SystemManager(SystemManager&&) = delete;
		SystemManager& operator=(const SystemManager&) = delete;
		SystemManager& operator=(SystemManager&&) = delete;

		template<typename T>
		static void AddSystem()
		{
			m_systems[typeid(T).hash_code()] = System::GetInstance<T>();
		}
		template<typename T>
		static void RemoveSystem()
		{
			m_systems.erase(typeid(T));
		}
		static void InitSystems();
		static void UpdateSystems();
		static void DestroySystems();
	private:
		static std::unordered_map<size_t, std::shared_ptr<System>> m_systems;
	};
}
#pragma once

#include<functional>
#include<unordered_map>
#include"RendererSystem.h"
class SystemManager
{
public:
	template<typename T>
	static void AddSystem()
	{
		m_systemUpdateCalls[typeid(T).hash_code()] = T::Update;
		m_systemInitCalls[typeid(T).hash_code()] = T::Init;
	}
	template<typename T>
	static void removeSystem()
	{
		m_systemUpdateCalls.erase(typeid(T));
		m_systemInitCalls.erase(typeid(T));
	}
	static void InitSystems();
	static void UpdateSystems();
private:
	static std::unordered_map<size_t, std::function<void()>> m_systemUpdateCalls;
	static std::unordered_map<size_t, std::function<void()>> m_systemInitCalls;
};
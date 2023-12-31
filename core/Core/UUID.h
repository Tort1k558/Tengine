#pragma once

#include<random>
#include<string>
#include<unordered_set>

#include"Core/Core.h"

namespace Tengine
{

	class TENGINE UUID
	{
	public:
		UUID();
		UUID(std::string_view id);
		std::string string() const;

		bool operator==(const UUID& other) const {
			return m_id == other.m_id;
		}
	private:
		std::string genId() const;
		std::string m_id;
		static std::unordered_set<std::string> m_ids;
		static std::random_device m_randomDevice;
		static std::mt19937_64 m_gen;
		static std::uniform_int_distribution<size_t> m_uniformDistribution;
	};
}

namespace std
{

	template<>
	struct hash<Tengine::UUID>
	{
		std::size_t operator()(const Tengine::UUID& uuid) const
		{
			return hash<string>{}(uuid.string());
		}
	};
}
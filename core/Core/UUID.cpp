#include "UUID.h"

#include"Core/Logger.h"

std::unordered_set<std::string> UUID::m_ids;
std::random_device UUID::m_randomDevice;
std::mt19937_64 UUID::m_gen(UUID::m_randomDevice());
std::uniform_int_distribution<size_t> UUID::m_uniformDistribution;

UUID::UUID()
{
	m_id = genId();
	while(m_ids.find(m_id) != m_ids.end())
	{
		m_id = genId();
	}
	m_ids.insert(m_id);
}

UUID::UUID(std::string_view id) :
	m_id(id)
{
	if (m_ids.find(m_id) != m_ids.end())
	{
		Logger::Warning("ERROR::UUID::Id {0} already exists", m_id);
	}

	while (m_ids.find(m_id) != m_ids.end())
	{
		m_id = genId();
	}
	m_ids.insert(m_id);
}

std::string UUID::string() const
{
	return m_id;
}

std::string UUID::genId()
{
	return "T" + std::to_string(m_uniformDistribution(m_gen));
}

#include "UUID.h"

std::unordered_set<size_t> UUID::m_ids;
std::random_device UUID::m_randomDevice;
std::mt19937_64 UUID::m_gen(UUID::m_randomDevice());
std::uniform_int_distribution<size_t> UUID::m_uniformDistribution;

UUID::UUID() :
	m_id(m_uniformDistribution(m_gen))
{
	while(m_ids.find(m_id) != m_ids.end())
	{
		m_id = m_uniformDistribution(m_gen);
	}
	m_ids.insert(m_id);
}

UUID::UUID(size_t id) :
	m_id(id)
{
	m_ids.insert(m_id);
}

std::string UUID::getID()
{
	return std::to_string(m_id);
}

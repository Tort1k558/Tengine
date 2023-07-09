#include "UUID.h"

std::random_device UUID::m_randomDevice;
std::mt19937_64 UUID::m_gen(UUID::m_randomDevice());
std::uniform_int_distribution<size_t> UUID::m_uniformDistribution;

UUID::UUID() :
	m_id(m_uniformDistribution(m_gen))
{

}

UUID::UUID(size_t id) :
	m_id(id)
{

}

std::string UUID::get()
{
	return std::to_string(m_id);
}

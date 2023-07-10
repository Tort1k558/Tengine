#pragma once

#include<random>
#include<string>
#include<unordered_set>

class UUID
{
public:
	UUID();
	UUID(size_t id);
	std::string getID();
private:
	size_t m_id;
	static std::unordered_set<size_t> m_ids;
	static std::random_device m_randomDevice;
	static std::mt19937_64 m_gen;
	static std::uniform_int_distribution<size_t> m_uniformDistribution;
};

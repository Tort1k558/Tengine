#pragma once

#include<random>
#include<string>

class UUID
{
public:
	UUID();
	UUID(size_t id);
	std::string get();
private:
	size_t m_id;
	static std::random_device m_randomDevice;
	static std::mt19937_64 m_gen;
	static std::uniform_int_distribution<size_t> m_uniformDistribution;
};

#pragma once
#include<vector>
#include<memory>
#include<string>
#include"ObjectManager.h"

class Object
{
public:
	Object();
	virtual ~Object() = default;

	template<typename T>
	void addComponent(T component)
	{

	}
	template<typename T>
	T getComponent()
	{

	}
	template<typename T>
	void removeComponent()
	{

	}
	void setName(const std::string& name);
	std::string getName() { return m_name; }
	unsigned int getId() { return m_id; }
	static std::shared_ptr<Object> Create();
private:
	unsigned int m_id;
	std::string m_name;
	static unsigned int s_ids;
};
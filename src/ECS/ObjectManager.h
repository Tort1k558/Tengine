#pragma once

#include<vector>
#include<memory>

#include"Object.h"
class Object;
class ObjectManager
{
public:
	static std::shared_ptr<Object> getObjectById(unsigned int id);
	static std::vector<std::shared_ptr<Object>> getAllObjects() { return m_objects; }
private:
	static void addObject(std::shared_ptr<Object> object);
	static std::vector<std::shared_ptr<Object>> m_objects;
	friend class Object;
};
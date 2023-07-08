#pragma once

#include<unordered_map>
#include<memory>

#include"Object.h"
class Object;
class ObjectManager
{
public:
	ObjectManager() = delete;
	ObjectManager(const ObjectManager&) = delete;
	ObjectManager& operator=(const ObjectManager&) = delete;

	static std::shared_ptr<Object> getObjectById(size_t id);
	static std::vector<std::shared_ptr<Object>> getAllObjects();
private:
	static void addObject(std::shared_ptr<Object> object);
	static std::unordered_map<size_t,std::shared_ptr<Object>> m_objects;
	friend class Object;
};
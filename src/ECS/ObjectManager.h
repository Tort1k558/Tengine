#pragma once

#include<unordered_map>
#include<memory>
#include<string>

#include"Object.h"
class Object;
class ObjectManager
{
public:
	ObjectManager() = delete;
	ObjectManager(const ObjectManager&) = delete;
	ObjectManager& operator=(const ObjectManager&) = delete;

	static std::shared_ptr<Object> getObjectById(std::string id);
	static std::vector<std::shared_ptr<Object>> getAllObjects();
private:
	static void addObject(std::shared_ptr<Object> object);
	static std::unordered_map<std::string,std::shared_ptr<Object>> m_objects;
	friend class Object;
};
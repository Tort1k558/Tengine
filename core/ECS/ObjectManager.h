#pragma once

#include<unordered_map>
#include<memory>
#include<string>

#include"ECS/Object.h"
#include"Scene/Scene.h"

class Object;

class ObjectManager
{
public:
	ObjectManager();

	std::shared_ptr<Object> getObjectByUUID(std::string id) const;
	std::vector<std::shared_ptr<Object>> getAllObjects() const;

	void addObject(std::shared_ptr<Object> object);
	void removeObjectByUUID(std::string idObject);
private:
	std::unordered_map<std::string,std::shared_ptr<Object>> m_objects;
};
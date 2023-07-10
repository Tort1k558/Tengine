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
	ObjectManager() = default;
	ObjectManager(const ObjectManager&) = default;
	ObjectManager(ObjectManager&&) = default;
	ObjectManager& operator=(const ObjectManager&) = default;
	ObjectManager& operator=(ObjectManager&&) = default;

	std::shared_ptr<Object> getObjectByUUID(std::string id);
	std::vector<std::shared_ptr<Object>> getAllObjects();
private:
	void addObject(std::shared_ptr<Object> object);
	void removeObject(std::string idObject);

	std::unordered_map<std::string,std::shared_ptr<Object>> m_objects;

	friend class Scene;
};
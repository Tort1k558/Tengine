#include "ObjectManager.h"

std::shared_ptr<Object> ObjectManager::getObjectByUUID(std::string id)
{
	if (m_objects.at(id))
	{
		return m_objects[id];
	}
	return nullptr;
}

std::vector<std::shared_ptr<Object>> ObjectManager::getAllObjects()
{
	std::vector<std::shared_ptr<Object>> objects;
	for (const auto& object : m_objects)
	{
		objects.push_back(object.second);
	}
	return objects;
}

void ObjectManager::addObject(std::shared_ptr<Object> object)
{
	m_objects[object->getId()] = object;
}

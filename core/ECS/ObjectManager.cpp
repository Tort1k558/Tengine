#include "ObjectManager.h"

ObjectManager::ObjectManager() :
	m_objects()
{

}

std::shared_ptr<Object> ObjectManager::getObjectByUUID(std::string id) const
{
	if (m_objects.at(id))
	{
		return m_objects.at(id);
	}
	return nullptr;
}

std::vector<std::shared_ptr<Object>> ObjectManager::getAllObjects() const
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

void ObjectManager::removeObject(std::string idObject)
{
	m_objects.erase(idObject);
}

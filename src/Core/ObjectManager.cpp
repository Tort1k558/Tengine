#include "ObjectManager.h"

std::vector<std::shared_ptr<Object>> ObjectManager::m_objects = {};

std::shared_ptr<Object> ObjectManager::getObjectById(unsigned int id)
{
	for (const auto& obj : m_objects)
	{
		if (obj->getId() == id)
		{
			return obj;
		}
	}
	return nullptr;
}

void ObjectManager::addObject(std::shared_ptr<Object> object)
{
	m_objects.push_back(object);
}

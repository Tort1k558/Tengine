#include "Scene.h"

Scene::Scene()
{
	static size_t counter = 0;
	m_name = "Scene" + std::to_string(counter);
}
std::string Scene::getName()
{
	return m_name;
}

std::shared_ptr<Scene> Scene::Create()
{
	return std::make_shared<Scene>();
}

void Scene::addObject(std::shared_ptr<Object> object)
{
	m_objects[object->getId()] = object;
}

void Scene::removeObjectByUUID(UUID id)
{
	m_objects.erase(id);
}

void Scene::removeObjectByName(std::string_view name)
{
	for (const auto& object : m_objects)
	{
		if (object.second->getName() == name)
		{
			removeObjectByUUID(object.second->getId());
			return;
		}
	}
}

void Scene::setName(std::string_view name)
{
	m_name = name;
}

std::shared_ptr<Object> Scene::getObjectByUUID(UUID id)
{
	if (m_objects.at(id))
	{
		return m_objects.at(id);
	}
	return nullptr;
}

std::shared_ptr<Object> Scene::getObjectByName(std::string_view name)
{
	for (const auto& object : m_objects)
	{
		if (object.second->getName() == name)
		{
			return object.second;
		}
	}
	return nullptr;
}

std::vector<std::shared_ptr<Object>> Scene::getAllObjects()
{
	std::vector<std::shared_ptr<Object>> objects;
	for (const auto& object : m_objects)
	{
		objects.push_back(object.second);
	}
	return objects;
}

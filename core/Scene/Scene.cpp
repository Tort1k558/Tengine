#include "Scene.h"

std::shared_ptr<Scene> Scene::Create()
{
	return std::make_shared<Scene>();
}

Scene::Scene() :
	m_objectManager({ std::make_shared<ObjectManager>() })
{

}

void Scene::addObject(std::shared_ptr<Object> object)
{
	m_objectManager->addObject(object);
}

void Scene::removeObjectByUUID(UUID id)
{
	m_objectManager->removeObjectByUUID(id.getID());
}

void Scene::removeObjectByName(std::string_view name)
{
	std::vector<std::shared_ptr<Object>> objects = m_objectManager->getAllObjects();
	for (const auto& object : objects)
	{
		if (object->getName() == name)
		{
			m_objectManager->removeObjectByUUID(object->getId());
		}
	}
}

std::shared_ptr<Object> Scene::getObjectByUUID(UUID id)
{
	return m_objectManager->getObjectByUUID(id.getID());
}

std::shared_ptr<Object> Scene::getObjectByName(std::string_view name)
{
	std::vector<std::shared_ptr<Object>> objects = m_objectManager->getAllObjects();
	for (const auto& object : objects)
	{
		if (object->getName() == name)
		{
			return object;
		}
	}
	return nullptr;
}

std::vector<std::shared_ptr<Object>> Scene::getAllObjects()
{
	return m_objectManager->getAllObjects();
}

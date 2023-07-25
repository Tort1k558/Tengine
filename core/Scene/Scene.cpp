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
	m_objectManager->removeObject(id.getID());
}

std::shared_ptr<Object> Scene::getObjectByUUID(UUID id)
{
	return m_objectManager->getObjectByUUID(id.getID());
}

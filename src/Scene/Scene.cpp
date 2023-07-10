#include "Scene.h"

std::shared_ptr<Scene> Scene::Create()
{
	return std::make_shared<Scene>();
}

Scene::Scene() :
	m_componentManager(std::make_shared<ComponentManager>()), m_objectManager({ std::make_shared<ObjectManager>() })
{

}

void Scene::addObject(std::shared_ptr<Object> object)
{
	m_objectManager->addObject(object);
}

void Scene::removeObject()
{

}

void Scene::getObjectByUUID(UUID id)
{

}

void Scene::addComponent(std::shared_ptr<Component> component, UUID idObject)
{
	m_componentManager->addComponent(component, idObject.getID());
}

std::vector<std::shared_ptr<Component>> Scene::getObjectComponentsByUUID(UUID idObject)
{
	return m_componentManager->getObjectComponents(idObject.getID());
}

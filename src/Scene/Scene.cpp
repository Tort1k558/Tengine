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
	object->m_parentScene = std::shared_ptr<Scene>(this);
}

void Scene::addComponent(std::shared_ptr<Component> component, UUID idObject)
{
	m_componentManager->addComponent(component, idObject.getID());
}

void Scene::removeObjectByUUID(UUID id)
{
	m_componentManager->removeAllObjectComponents(id.getID());
	m_objectManager->removeObject(id.getID());
}

std::shared_ptr<Object> Scene::getObjectByUUID(UUID id)
{
	return m_objectManager->getObjectByUUID(id.getID());
}

std::vector<std::shared_ptr<Component>> Scene::getObjectComponentsByUUID(UUID idObject)
{
	return m_componentManager->getObjectComponents(idObject.getID());
}

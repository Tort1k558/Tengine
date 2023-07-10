#pragma once

#include"ECS/Component.h"
#include"Core/UUID.h"
#include"ECS/Object.h"
#include"ECS/ComponentManager.h"
#include"ECS/ObjectManager.h"

class Object;
class ObjectManager;
class ComponentManager;

class Scene
{
public:
	Scene();
	void addObject(std::shared_ptr<Object> object);
	void removeObjectByUUID(UUID id);
	std::shared_ptr<Object> getObjectByUUID(UUID id);

	template<typename T>
	std::vector<std::shared_ptr<T>> getComponents()
	{
		return m_componentManager->getComponents<T>();
	}

	static std::shared_ptr<Scene> Create();
private:
	std::shared_ptr<ObjectManager> m_objectManager;
	std::shared_ptr<ComponentManager> m_componentManager;

	void addComponent(std::shared_ptr<Component> component, UUID idObject);

	template<typename T>
	void removeComponent(UUID idObject)
	{
		m_componentManager->removeComponent<T>(idObject.getID());
	}

	std::vector<std::shared_ptr<Component>> getObjectComponentsByUUID(UUID idObject);

	friend class Object;
};

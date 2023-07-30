#pragma once

#include"ECS/Component.h"
#include"Core/UUID.h"
#include"ECS/Object.h"
#include"ECS/ObjectManager.h"
#include"ECS/Components/Transform.h"

class Object;
class ObjectManager;

class Scene
{
public:
	Scene();
	void addObject(std::shared_ptr<Object> object);
	void removeObjectByUUID(UUID id);
	void removeObjectByName(std::string_view name);

	std::shared_ptr<Object> getObjectByUUID(UUID id);
	std::shared_ptr<Object> getObjectByName(std::string_view name);
	std::vector<std::shared_ptr<Object>> getAllObjects();
	template<typename T>
	std::vector<std::shared_ptr<T>> getComponents();

	static std::shared_ptr<Scene> Create();
private:
	std::shared_ptr<ObjectManager> m_objectManager;
};

template<typename T>
inline std::vector<std::shared_ptr<T>> Scene::getComponents()
{
	std::vector<std::shared_ptr<T>> components;
	std::vector<std::shared_ptr<Object>> objects = m_objectManager->getAllObjects();
	for (auto& object : objects)
	{
		std::shared_ptr<T> comp = object->getComponent<T>();
		if (comp)
		{
			components.push_back(comp);
		}
	}
	return components;
}

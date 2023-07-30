#pragma once

#include"ECS/Component.h"
#include"Core/UUID.h"
#include"ECS/Object.h"
#include"ECS/Components/Transform.h"

class Object;

class Scene
{
public:
	Scene() = default;

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
	std::unordered_map<std::string, std::shared_ptr<Object>> m_objects;
};

template<typename T>
inline std::vector<std::shared_ptr<T>> Scene::getComponents()
{
	std::vector<std::shared_ptr<T>> components;
	for (auto& object : m_objects)
	{
		std::shared_ptr<T> comp = object.second->getComponent<T>();
		if (comp)
		{
			components.push_back(comp);
		}
	}
	return components;
}

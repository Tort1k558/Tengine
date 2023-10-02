#include "Scene.h"

namespace Tengine
{

	Scene::Scene()
	{
		static size_t counter = 0;
		m_name = "Scene" + std::to_string(counter);
	}
	std::string Scene::getName() const
	{
		return m_name;
	}

	std::filesystem::path Scene::getPath()
	{
		return m_path;
	}

	std::shared_ptr<Scene> Scene::Create()
	{
		return std::shared_ptr<Scene>(new Scene());
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

	void Scene::setPath(std::filesystem::path path)
	{
		m_path = path;
	}

	void Scene::setMainCamera(std::shared_ptr<Camera> camera)
	{
		m_mainCamera = camera;
	}

	std::shared_ptr<Object> Scene::getObjectByUUID(UUID id) const
	{
		if (m_objects.find(id) != m_objects.end())
		{
			return m_objects.at(id);
		}
		return nullptr;
	}

	std::shared_ptr<Object> Scene::getObjectByName(std::string_view name) const
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

	std::vector<std::shared_ptr<Object>> Scene::getAllObjects() const
	{
		std::vector<std::shared_ptr<Object>> objects;
		for (const auto& object : m_objects)
		{
			objects.push_back(object.second);
		}
		return objects;
	}
	std::shared_ptr<Camera> Scene::getMainCamera()
	{
		return m_mainCamera;
	}
}
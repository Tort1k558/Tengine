#pragma once

#include"Scene/Component.h"
#include"Core/UUID.h"
#include"Scene/Object.h"
#include"Components/Camera.h"
#include"Components/Transform.h"

namespace Tengine
{

	class Object;

	class TENGINE Scene
	{
	public:

		void addObject(std::shared_ptr<Object> object);
		void removeObjectByUUID(UUID id);
		void removeObjectByName(std::string_view name);
		void setName(std::string_view name);
		void setPath(std::filesystem::path path);
		void setMainCamera(std::shared_ptr<Camera> camera);
		std::shared_ptr<Object> getObjectByUUID(UUID id) const;
		std::shared_ptr<Object> getObjectByName(std::string_view name) const;
		std::vector<std::shared_ptr<Object>> getAllObjects() const;
		std::shared_ptr<Camera> getMainCamera();
		template<typename T>
		std::vector<std::shared_ptr<T>> getComponents() const;
		std::string getName() const;
		std::filesystem::path getPath();

		static std::shared_ptr<Scene> Create();
	private:
		Scene();
		std::unordered_map<UUID, std::shared_ptr<Object>> m_objects;
		std::string m_name;
		std::filesystem::path m_path;
		std::shared_ptr<Camera> m_mainCamera;
	};

	template<typename T>
	inline std::vector<std::shared_ptr<T>> Scene::getComponents() const
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
}

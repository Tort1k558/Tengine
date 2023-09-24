#pragma once
#include<vector>
#include<memory>
#include<string>

#include<unordered_map>

#include"Core/UUID.h"
#include"ECS/Component.h"
#include"Core/Core.h"

namespace Tengine
{

	class Scene;
	class Component;

	class TENGINE Object : public std::enable_shared_from_this<Object>
	{
	public:
		Object();
		Object(UUID uuid);
		virtual ~Object() = default;

		void addComponent(std::shared_ptr<Component> component);
		template<typename T>
		void removeComponent();

		template<typename T>
		std::shared_ptr<T> getComponent() const;

		template<typename T>
		bool hasComponent() const;

		std::vector<std::shared_ptr<Component>> getComponents();

		void setName(const std::string& name);
		std::string getName() const;

		UUID getId() const;

		static std::shared_ptr<Object> Create();
		static std::shared_ptr<Object> Create(UUID id);

	private:
		UUID m_id;
		std::string m_name;
		std::vector<std::shared_ptr<Component>> m_components;
	};

	template<typename T>
	inline void Object::removeComponent()
	{
		for (size_t i = 0; i < m_components.size(); i++)
		{
			if (std::dynamic_pointer_cast<T>(m_components[i]))
			{
				m_components[i]->setParent(nullptr);
				m_components.erase(i);
			}
		}
	}


	template<typename T>
	inline std::shared_ptr<T> Object::getComponent() const
	{
		for (size_t i = 0; i < m_components.size(); i++)
		{
			std::shared_ptr<T> ptr = std::dynamic_pointer_cast<T>(m_components[i]);
			if (ptr)
			{
				return ptr;
			}
		}
		return nullptr;
	}

	template<typename T>
	inline bool Object::hasComponent() const
	{
		for (size_t i = 0; i < m_components.size(); i++)
		{
			if (std::dynamic_pointer_cast<T>(m_components[i]))
			{
				return true;
			}
		}
		return false;
	}
}
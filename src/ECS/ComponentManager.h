#pragma once

#include<vector>
#include<memory>

#include"ECS/Component.h"
#include"ECS/Object.h"

class Scene;

class ComponentManager
{
public:
	ComponentManager() = default;
	ComponentManager(const ComponentManager&) = default;
	ComponentManager(ComponentManager&&) = default;
	ComponentManager& operator=(const ComponentManager&) = default;
	ComponentManager& operator=(ComponentManager&&) = default;

	template<typename T>
	std::vector<std::shared_ptr<T>> getComponents()
	{
		std::vector<std::shared_ptr<T>> components;
		for (const auto& comp : m_components)
		{
			if (std::shared_ptr<T> cmp = std::dynamic_pointer_cast<T>(comp.second))
			{
				components.push_back(cmp);
			}
		}
		return components;
	}
	std::vector<std::shared_ptr<Component>> getObjectComponents(std::string idObject);
	std::vector<std::shared_ptr<Component>> getAllComponents();
private:
	void addComponent(std::shared_ptr<Component> component, std::string idObject);

	template<typename T>
	void removeComponent(std::string idObject)
	{
		for (size_t i = 0; i < m_components.size(); i++)
		{
			if (m_components[i].first == idObject)
			{
				if (std::shared_ptr<T> cmp = std::dynamic_pointer_cast<T>(m_components[i].second))
				{
					m_components.erase(m_components.begin() + i);
					break;
				}
			}
		}
	}

	std::vector<std::pair<std::string, std::shared_ptr<Component>>> m_components;
	
	friend class Scene;
};
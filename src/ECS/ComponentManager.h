#pragma once

#include<vector>
#include<memory>

#include"ECS/Component.h"
#include"ECS/Object.h"

class Scene;

class ComponentManager
{
public:
	ComponentManager();

	std::vector<std::shared_ptr<Component>> getObjectComponents(std::string idObject) const;
	std::vector<std::shared_ptr<Component>> getAllComponents() const;

	void addComponent(std::shared_ptr<Component> component, std::string idObject);
	void removeAllObjectComponents(std::string idObject);

	template<typename T>
	std::vector<std::shared_ptr<T>> getComponents() const;
	template<typename T>
	void removeComponent(std::string idObject);
private:
	std::vector<std::pair<std::string, std::shared_ptr<Component>>> m_components;

};

template<typename T>
std::vector<std::shared_ptr<T>> ComponentManager::getComponents() const
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

template<typename T>
void ComponentManager::removeComponent(std::string idObject)
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
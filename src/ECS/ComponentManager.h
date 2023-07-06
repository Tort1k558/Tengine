#pragma once

#include<vector>
#include<memory>

#include"Component.h"
#include"Object.h"
class ComponentManager
{
public:
	template<typename T>
	static std::vector<std::shared_ptr<T>> getComponents()
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
	static std::vector<std::shared_ptr<Component>> getAllObjectComponents(unsigned int idObjcet);
	static std::vector<std::shared_ptr<Component>> getAllComponents();
private:
	static void addComponent(std::shared_ptr<Component> component, unsigned int idObject);

	template<typename T>
	static void removeComponent(unsigned int idObject)
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

	static std::vector<std::pair<unsigned int, std::shared_ptr<Component>>> m_components;

	friend class Object;
};
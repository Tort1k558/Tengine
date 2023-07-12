#include"ComponentManager.h"

ComponentManager::ComponentManager() :
	m_components()
{
}

std::vector<std::shared_ptr<Component>> ComponentManager::getObjectComponents(std::string idObjcet) const
{
	std::vector<std::shared_ptr<Component>> components;
	for (const auto& comp : m_components)
	{
		if (comp.first == idObjcet)
		{
			components.push_back(comp.second);
		}
	}
	return components;
}

std::vector<std::shared_ptr<Component>> ComponentManager::getAllComponents() const
{
	std::vector<std::shared_ptr<Component>> components;
	for (const auto& comp : m_components)
	{
		components.push_back(comp.second);
	}
	return components;
}

void ComponentManager::addComponent(std::shared_ptr<Component> component, std::string idObject)
{
	m_components.push_back({ idObject,component });
}

void ComponentManager::removeAllObjectComponents(std::string idObject)
{
	for (size_t i = 0; i < m_components.size(); i++)
	{
		if (m_components[i].first == idObject)
		{
			m_components.erase(m_components.begin() + i);
		}
	}
}

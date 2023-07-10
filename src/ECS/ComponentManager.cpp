#include"ComponentManager.h"

std::vector<std::pair<std::string, std::shared_ptr<Component>>> ComponentManager::m_components;

std::vector<std::shared_ptr<Component>> ComponentManager::getAllObjectComponents(std::string idObjcet)
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

std::vector<std::shared_ptr<Component>> ComponentManager::getAllComponents()
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

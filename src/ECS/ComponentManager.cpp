#include"ComponentManager.h"

ComponentManager::ComponentManager() :
	m_components()
{
}

std::vector<std::shared_ptr<Component>> ComponentManager::getObjectComponents(std::string idObject) const
{
	std::vector<std::shared_ptr<Component>> components;
	auto objectComponents = m_components.at(idObject);
	for (auto& component : objectComponents)
	{
		components.push_back(component.second);
	}
	return components;
}

std::vector<std::shared_ptr<Component>> ComponentManager::getAllComponents() const
{
	std::vector<std::shared_ptr<Component>> components;
	for (const auto& object : m_components)
	{
		for (const auto& comp : object.second)
		{
			components.push_back(comp.second);
		}
	}
	return components;
}

void ComponentManager::addComponent(std::shared_ptr<Component> component, std::string idObject)
{
	m_components[idObject][typeid(component).hash_code()] = component;
}

void ComponentManager::removeAllObjectComponents(std::string idObject)
{
	m_components.erase(idObject);
}

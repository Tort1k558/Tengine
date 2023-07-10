#pragma once
#include<vector>
#include<memory>
#include<string>

#include"ObjectManager.h"
#include"ComponentManager.h"

#include"Core/UUID.h"
class Object
{
public:
	Object();
	virtual ~Object() = default;

	template<typename T>
	void addComponent(std::shared_ptr<T> component)
	{
		ComponentManager::addComponent(component, m_id.getID());
	}
	template<typename T>
	std::shared_ptr<T> getComponent()
	{
		std::vector<std::shared_ptr<Component>> components = ComponentManager::getAllObjectComponents(m_id);
		for (const auto& comp : components)
		{
			if (std::shared_ptr<T> cmp = std::dynamic_pointer_cast<T>(comp))
			{
				return cmp;
			}
		}
		return nullptr;
	}
	template<typename T>
	void removeComponent()
	{
		ComponentManager::removeComponent<T>(m_id);
	}
	void setName(const std::string& name);
	std::string getName() { return m_name; }
	std::string getId() { return m_id.getID(); }
	static std::shared_ptr<Object> Create();
private:
	UUID m_id;
	std::string m_name;
};
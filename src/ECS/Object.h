#pragma once
#include<vector>
#include<memory>
#include<string>

#include"Core/UUID.h"

class Scene;

class Object
{
public:
	Object();
	virtual ~Object() = default;

	template<typename T>
	void addComponent(std::shared_ptr<T> component)
	{
		m_parentScene->addComponent(component, m_id);
	}

	template<typename T>
	std::shared_ptr<T> getComponent()
	{
		std::vector<std::shared_ptr<Component>> components = m_parentScene->getObjectComponents(m_id);
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
		m_parentScene->removeComponent<T>(m_id);
	}

	void setName(const std::string& name);
	std::string getName() { return m_name; }
	std::string getId() { return m_id.getID(); }
	static std::shared_ptr<Object> Create();
private:
	UUID m_id;
	std::string m_name;
	std::shared_ptr<Scene> m_parentScene;

	friend class Scene;
};
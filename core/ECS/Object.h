#pragma once
#include<vector>
#include<memory>
#include<string>

#include<unordered_map>

#include"Core/UUID.h"
#include"ECS/Component.h"
class Scene;
class Component;

class Object : public std::enable_shared_from_this<Object>
{
public:
	Object();
	Object(UUID uuid);
	virtual ~Object() = default;

	template<typename T>
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
	std::unordered_map<size_t, std::shared_ptr<Component>> m_components;
};

template<typename T>
inline void Object::addComponent(std::shared_ptr<Component> component)
{
	if (component)
	{
		component->setParent(shared_from_this());
		m_components[typeid(T).hash_code()] = std::dynamic_pointer_cast<T>(component);
	}
}

template<typename T>
inline void Object::removeComponent()
{
	if (m_components.find(typeid(T).hash_code()) != m_components.end())
	{
		m_components[typeid(T).hash_code()]->setParent(nullptr);
		m_components.erase(typeid(T).hash_code());
	}
}


template<typename T>
inline std::shared_ptr<T> Object::getComponent() const
{
	if (m_components.find(typeid(T).hash_code()) == m_components.end())
	{
		return nullptr;
	}
	std::shared_ptr<T> component = std::dynamic_pointer_cast<T>(m_components.at(typeid(T).hash_code()));
	return component;
}

template<typename T>
inline bool Object::hasComponent() const
{
	if (m_components.find(typeid(T).hash_code()) == m_components.end())
	{
		return false;
	}
	return true;
}

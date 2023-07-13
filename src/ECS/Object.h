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
	virtual ~Object() = default;

	template<typename T>
	void addComponent(std::shared_ptr<Component> component);
	template<typename T>
	void removeComponent();

	template<typename T>
	std::shared_ptr<T> getComponent();

	void setName(const std::string& name);
	std::string getName() { return m_name; }
	std::string getId() { return m_id.getID(); }
	static std::shared_ptr<Object> Create();

private:
	UUID m_id;
	std::string m_name;
	std::unordered_map<size_t, std::shared_ptr<Component>> m_components;

	friend class Scene;
};

template<typename T>
inline void Object::addComponent(std::shared_ptr<Component> component)
{
	component->setParent(shared_from_this());
	m_components[typeid(T).hash_code()] = std::dynamic_pointer_cast<T>(component);
}

template<typename T>
inline void Object::removeComponent()
{
	m_components[typeid(T).hash_code()]->setParent(nullptr);
	m_components.erase(typeid(T).hash_code());
}


template<typename T>
inline std::shared_ptr<T> Object::getComponent()
{
	std::shared_ptr<T> component = std::dynamic_pointer_cast<T>(m_components[typeid(T).hash_code()]);
	return component;
}
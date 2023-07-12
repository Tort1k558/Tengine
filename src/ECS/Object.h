#pragma once
#include<vector>
#include<memory>
#include<string>

#include<unordered_map>

#include"Core/UUID.h"
#include"ECS/Component.h"
class Scene;

class Object
{
public:
	Object();
	virtual ~Object() = default;

	template<typename T>
	void addComponent(std::shared_ptr<T> component);

	template<typename T>
	std::shared_ptr<T> getComponent();

	template<typename T>
	void removeComponent();

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
inline void Object::addComponent(std::shared_ptr<T> component)
{
	m_components[typeid(T).hash_code()] = component;
}

template<typename T>
inline std::shared_ptr<T> Object::getComponent()
{
	std::shared_ptr<T> component = std::dynamic_pointer_cast<T>(m_components[typeid(T).hash_code()]);
	return component;
}

template<typename T>
inline void Object::removeComponent()
{
	m_components.erase(typeid(T).hash_code());
}

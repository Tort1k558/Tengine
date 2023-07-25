#include "Object.h"

#include "Scene/SceneManager.h"
#include "Components/Transform.h"

Object::Object() :
	m_id(),m_components({})
{
	static size_t counterObject = 0;
	m_name = "Object" + std::to_string(counterObject);
	counterObject++;
}

void Object::setName(const std::string& name)
{
	m_name = name;
}

std::shared_ptr<Object> Object::Create()
{
	std::shared_ptr<Object> object = std::make_shared<Object>();
	object->addComponent<Transform>(Component::Create<Transform>());
	SceneManager::GetCurrentScene()->addObject(object);
	return object;
}

std::vector<std::shared_ptr<Component>> Object::getComponents()
{
	std::vector<std::shared_ptr<Component>> components;
	for (const auto& component : m_components)
	{
		components.push_back(component.second);
	}
	return components;
}
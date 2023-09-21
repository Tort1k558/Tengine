#include "Object.h"

#include "Scene/SceneManager.h"
#include "Components/Transform.h"

namespace Tengine
{

	Object::Object() :
		m_id(), m_components()
	{
		static size_t counterObject = 0;
		m_name = "Object" + std::to_string(counterObject);
		counterObject++;
	}

	Object::Object(UUID id) :
		m_components()
	{
		m_id = id;
	}

	void Object::setName(const std::string& name)
	{
		m_name = name;
	}

	std::string Object::getName() const
	{
		return m_name;
	}

	UUID Object::getId() const
	{
		return m_id;
	}

	std::shared_ptr<Object> Object::Create()
	{
		std::shared_ptr<Object> object = std::make_shared<Object>();
		object->addComponent<Transform>(Component::Create<Transform>());
		SceneManager::GetCurrentScene()->addObject(object);
		return object;
	}

	std::shared_ptr<Object> Object::Create(UUID id)
	{
		std::shared_ptr<Object> object = std::make_shared<Object>(id);
		object->addComponent<Transform>(Component::Create<Transform>());
		SceneManager::GetCurrentScene()->addObject(object);
		return object;
	}

	std::vector<std::shared_ptr<Component>> Object::getComponents()
	{
		return m_components;
	}
}
#include "Object.h"

#include "Scene/SceneManager.h"
#include "Components/Transform.h"

Object::Object() :
	m_id(), m_name(), m_parentScene(nullptr)
{
}

void Object::setName(const std::string& name)
{
	m_name = name;
}

std::shared_ptr<Object> Object::Create()
{
	std::shared_ptr<Object> object = std::make_shared<Object>();
	object->m_parentScene = SceneManager::GetCurrentScene();

	object->addComponent(Component::Create<Transform>());
	return object;
}

#include "Object.h"

#include "Scene/SceneManager.h"
#include "Components/Transform.h"

Object::Object() :
	m_id(), m_name()
{
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

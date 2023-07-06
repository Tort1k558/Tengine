#include "Object.h"

unsigned int Object::s_ids = 0;

Object::Object() :
	m_id(s_ids++)
{
}

void Object::setName(const std::string& name)
{
	m_name = name;
}

std::shared_ptr<Object> Object::Create()
{
	std::shared_ptr<Object> object = std::make_shared<Object>();
	ObjectManager::addObject(object);
	return object;
}

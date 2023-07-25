#include "Component.h"

void Component::setParent(std::shared_ptr<Object> parent)
{
	m_parent = parent;
}

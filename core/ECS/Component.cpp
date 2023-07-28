#include "Component.h"

void Component::setParent(std::shared_ptr<Object> parent)
{
	m_parent = parent;
}

void DisplayInfo::addElement(std::shared_ptr<DisplayInfoElement> element)
{
	m_elements.push_back(element);
}

void DisplayInfo::setComponentName(const std::string& name)
{
	m_componentName = name;
}
std::string DisplayInfo::getComponentName()
{
	return m_componentName;
}

std::vector<std::shared_ptr<DisplayInfoElement>> DisplayInfo::getElements()
{
	return m_elements;
}
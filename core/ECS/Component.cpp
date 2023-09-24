#include "Component.h"
namespace Tengine
{
	void Component::setParent(std::shared_ptr<Object> parent)
	{
		m_parent = parent;
	}

	std::shared_ptr<Object> Component::getParent() const
	{
		return m_parent.lock();
	}

	DisplayInfo Component::getDisplayInfo()
	{
		return m_displayInfo;
	}

	void DisplayInfo::addElement(std::shared_ptr<DisplayInfoElement> element)
	{
		m_elements.push_back(element);
	}

	void DisplayInfo::setComponentName(const std::string& name)
	{
		m_componentName = name;
	}
	std::string DisplayInfo::getComponentName() const
	{
		return m_componentName;
	}

	std::vector<std::shared_ptr<DisplayInfoElement>> DisplayInfo::getElements() const
	{
		return m_elements;
	}
}
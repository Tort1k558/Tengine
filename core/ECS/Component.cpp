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

	ComponentInfo Component::getInfo()
	{
		return ComponentInfo();
	}

	void ComponentInfo::addElement(std::shared_ptr<FieldInfo> element)
	{
		m_elements.push_back(element);
	}

	void ComponentInfo::setComponentName(const std::string& name)
	{
		m_componentName = name;
	}
	std::string ComponentInfo::getComponentName() const
	{
		return m_componentName;
	}

	std::vector<std::shared_ptr<FieldInfo>> ComponentInfo::getElements() const
	{
		return m_elements;
	}
}
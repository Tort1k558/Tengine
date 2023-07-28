#pragma once

#include<memory>
#include<vector>
#include<functional>
#include"ECS/Object.h"

enum class DisplayTypeElement
{
	None,
	Int,
	Bool,
	Float,
	Double,
	Vec2,
	Vec3,
	Vec4
};

struct DisplayInfoElement
{
	DisplayTypeElement type;
	std::string name;
	void* data;
	float minValue;
	float maxValue;
};

class DisplayInfo
{
public:
	DisplayInfo() = default;
	void addElement(DisplayInfoElement element);
	void setComponentName(const std::string& name);
	std::string getComponentName();
	std::vector<DisplayInfoElement> getElements();
private:
	std::vector<DisplayInfoElement> m_elements;
	std::string m_componentName;
};

class Object;

class Component
{
public:
	virtual ~Component() = default;

	void setParent(std::shared_ptr<Object> parent);
	std::shared_ptr<Object> getParent() { return m_parent.lock(); }

	template<typename T, typename... Args>
	static std::shared_ptr<T> Create(Args... args);

	std::shared_ptr<DisplayInfo> getDisplayInfo() { return m_displayInfo; }
	bool hasDisplayInfo() { return m_displayInfo.get() != nullptr; }
protected:
	std::shared_ptr<DisplayInfo> m_displayInfo;
private:
	std::weak_ptr<Object> m_parent;
};

template<typename T, typename ...Args>
inline std::shared_ptr<T> Component::Create(Args ...args)
{
	return std::make_shared<T>(args...);
}
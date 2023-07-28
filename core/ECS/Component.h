#pragma once

#include<memory>
#include<vector>
#include<functional>
#include"ECS/Object.h"

enum class DisplayTypeElement
{
	None,
	Slider,
	Slider2,
	Slider3,
	Slider4,
	Combo
};

struct DisplayInfoElement
{
	virtual ~DisplayInfoElement() = default;
	DisplayTypeElement type;
	std::string name;
	std::function<void()> function;
};

struct DisplayInfoElementSlider : public DisplayInfoElement
{
	void* data;
	float minValue;
	float maxValue;
};

struct DisplayInfoElementCombo : public DisplayInfoElement
{
	std::vector<std::string> elements;
	int* currentElement;
	float minValue;
	float maxValue;
};

class DisplayInfo
{
public:
	DisplayInfo() = default;
	void addElement(std::shared_ptr<DisplayInfoElement> element);
	void setComponentName(const std::string& name);
	std::string getComponentName();
	std::vector< std::shared_ptr<DisplayInfoElement>> getElements();
private:
	std::vector<std::shared_ptr<DisplayInfoElement>> m_elements;
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

	virtual DisplayInfo getDisplayInfo() { return DisplayInfo(); }
	virtual bool hasDisplayInfo() { return false; }
private:
	std::weak_ptr<Object> m_parent;
};

template<typename T, typename ...Args>
inline std::shared_ptr<T> Component::Create(Args ...args)
{
	return std::make_shared<T>(args...);
}
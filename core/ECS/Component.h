#pragma once

#include<memory>

#include"ECS/Object.h"
#include"Utils/EditorElements/DisplayInfo.h"

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
#pragma once

#include<memory>

#include"ECS/Object.h"

class Object;

class Component
{
public:
	virtual ~Component() = default;
	template<typename T, typename... Args>
	static std::shared_ptr<T> Create(Args... args)
	{
		return std::make_shared<T>(args...);
	}
	void setParent(std::shared_ptr<Object> parent);
	std::shared_ptr<Object> getParent() { return m_parent.lock(); }
private:
	std::weak_ptr<Object> m_parent;
};
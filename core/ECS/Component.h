#pragma once

//#define TRACEABLE \
//	template<typename T> \
//	friend class reflang::Class;

#include<memory>
#include<nlohmann/json.hpp>

#include"reflang.hpp"
#include"ECS/Object.h"
#include"Utils/EditorElements/DisplayInfo.h"

class Object;

class Component
{
public:
	virtual ~Component() = default;

	void setParent(std::shared_ptr<Object> parent);
	std::shared_ptr<Object> getParent() const;

	template<typename T, typename... Args>
	static std::shared_ptr<T> Create(Args... args);

	virtual DisplayInfo getDisplayInfo() { return DisplayInfo(); }
	virtual void serialize(nlohmann::json& data) {}
	virtual void deserialize(nlohmann::json& data,std::shared_ptr<Object> object) {}
private:
	std::weak_ptr<Object> m_parent;
};

template<typename T, typename ...Args>
inline std::shared_ptr<T> Component::Create(Args ...args)
{
	return std::make_shared<T>(args...);
}
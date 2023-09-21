#pragma once

//#define TRACEABLE \
//	template<typename T> \
//	friend class reflang::Class;

#include<memory>
#include<nlohmann/json.hpp>

#include"ECS/Object.h"
#include"Utils/EditorElements/DisplayInfo.h"

namespace Tengine
{
	class Object;
	
	class TENGINE Component
	{
	public:
		virtual ~Component() = default;

		void setParent(std::shared_ptr<Object> parent);
		std::shared_ptr<Object> getParent() const;

		template<typename T, typename... Args>
		static std::shared_ptr<T> Create(Args... args);

		virtual DisplayInfo getDisplayInfo() { return DisplayInfo(); }
		virtual void serialize(nlohmann::json& data) {}
	private:
		std::weak_ptr<Object> m_parent;
	};

	template<typename T, typename ...Args>
	inline std::shared_ptr<T> Component::Create(Args ...args)
	{
		return std::make_shared<T>(args...);
	}
}
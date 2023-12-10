#pragma once

#include<memory>
#include<nlohmann/json.hpp>

#include"ComponentInfo.h"

#define SERIALIZABLE ComponentInfo getInfo() override;

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

		virtual ComponentInfo getInfo();
	private:
		std::weak_ptr<Object> m_parent;
	protected:
	};

	template<typename T, typename ...Args>
	inline std::shared_ptr<T> Component::Create(Args ...args)
	{
		return std::make_shared<T>(args...);
	}
}
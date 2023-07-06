#pragma once

#include<memory>

class Component
{
public:
	virtual ~Component() = default;
	template<typename T, typename... Args>
	static std::shared_ptr<T> Create(Args... args)
	{
		return std::make_shared<T>(args...);
	}
private:

};
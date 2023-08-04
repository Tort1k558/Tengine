#ifndef REFLANG_METADATA_C__USERS_TORT1K_DESKTOP_TENGINE_CORE_UTILS_REFLECTION_HPP
#define REFLANG_METADATA_C__USERS_TORT1K_DESKTOP_TENGINE_CORE_UTILS_REFLECTION_HPP

#include <string>
#include "reflang.hpp"
#include "C:/Users/Tort1k/Desktop/Tengine/core/ECS/Components/Transform.h"

namespace reflang
{


template <>
class Class<Transform> : public IClass
{
public:
	static const constexpr int FieldCount = 3;
	static const constexpr int StaticFieldCount = 0;
	static const constexpr int MethodCount = 0;
	static const constexpr int StaticMethodCount = 0;

	std::vector<FieldData> GetFieldData() const override;

	int GetFieldCount() const override;
	Reference GetField(
			const Reference& o, const std::string& name) const override;

	int GetStaticFieldCount() const override;
	Reference GetStaticField(const std::string& name) const override;

	int GetMethodCount() const override;
	std::vector<std::unique_ptr<IMethod>> GetMethod(
			const std::string& name) const override;

	int GetStaticMethodCount() const override;
	std::vector<std::unique_ptr<IFunction>> GetStaticMethod(
			const std::string& name) const override;

	const std::string& GetName() const override;

	// Calls T::operator() on each field of 'Transform'.
	// Works well with C++14 generic lambdas.
	template <typename T>
	static void IterateFields(const Transform& c, T t);

	template <typename T>
	static void IterateFields(Transform& c, T t);

	template <typename T>
	static void IterateStaticFields(T t);

	
};

template <typename T>
void Class<Transform>::IterateFields(const Transform& c, T t)
{
	t(c.m_position);
	t(c.m_rotation);
	t(c.m_scale);
}

template <typename T>
void Class<Transform>::IterateFields(Transform& c, T t)
{
	t(c.m_position);
	t(c.m_rotation);
	t(c.m_scale);
}

template <typename T>
void Class<Transform>::IterateStaticFields(T t)
{
}




}  // namespace reflang

#endif //REFLANG_METADATA_C__USERS_TORT1K_DESKTOP_TENGINE_CORE_UTILS_REFLECTION_HPP

#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <type_traits>

namespace reflang
{


const int Class<Transform>::FieldCount;
const int Class<Transform>::StaticFieldCount;
const int Class<Transform>::MethodCount;
const int Class<Transform>::StaticMethodCount;

std::vector<FieldData> Class<Transform>::GetFieldData() const
{
	std::vector<FieldData> data;
	data.push_back({"m_position", "Vec3", static_cast<AccessModifier>(1)}); 
	data.push_back({"m_rotation", "Vec3", static_cast<AccessModifier>(1)}); 
	data.push_back({"m_scale", "Vec3", static_cast<AccessModifier>(1)}); 
	return data;
}

int Class<Transform>::GetFieldCount() const
{
	return FieldCount;
}

Reference Class<Transform>::GetField(const Reference& r, const std::string& name) const
{
	if (r.IsT<Transform>())
	{
		Transform& o = r.GetT<Transform>();
		if (name == "m_position")
		{
			return Reference(o.m_position);
		}
		if (name == "m_rotation")
		{
			return Reference(o.m_rotation);
		}
		if (name == "m_scale")
		{
			return Reference(o.m_scale);
		}
	}
	else if (r.IsT<const Transform>())
	{
		const Transform& o = r.GetT<const Transform>();
		if (name == "m_position")
		{
			return Reference(o.m_position);
		}
		if (name == "m_rotation")
		{
			return Reference(o.m_rotation);
		}
		if (name == "m_scale")
		{
			return Reference(o.m_scale);
		}
	}
	else
	{
		throw Exception("Invalid Reference passed to GetField().");
	}
	throw Exception("Invalid name passed to GetField().");
}

int Class<Transform>::GetStaticFieldCount() const
{
	return StaticFieldCount;
}

Reference Class<Transform>::GetStaticField(const std::string& name) const
{
	throw Exception("Invalid name passed to GetStaticField().");
}

int Class<Transform>::GetMethodCount() const
{
	return MethodCount;
}

std::vector<std::unique_ptr<IMethod>> Class<Transform>::GetMethod(const std::string& name) const
{
	std::vector<std::unique_ptr<IMethod>> results;

	return results;
}

int Class<Transform>::GetStaticMethodCount() const
{
	return StaticMethodCount;
}

std::vector<std::unique_ptr<IFunction>> Class<Transform>::GetStaticMethod(
		const std::string& name) const
{
	std::vector<std::unique_ptr<IFunction>> results;

	return results;
}

static const std::string Transform_name = "Transform";

const std::string& Class<Transform>::GetName() const
{
	return Transform_name;
}



namespace
{
	// Object to auto-register Transform.
	struct Transform_registrar
	{
		Transform_registrar()
		{
			::reflang::registry::internal::Register(
					std::make_unique<Class<Transform>>());
		}
	} Transform_instance;
}

}  // namespace reflang

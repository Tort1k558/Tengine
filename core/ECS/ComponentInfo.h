#pragma once

#include<vector>
#include<functional>
#include<string>
#include<filesystem>

#include"Renderer/Texture.h"

namespace Tengine
{
	enum class FieldType
	{
		None,
		Float,
		Vec2,
		Vec3,
		Vec4,
		Enum,
		CollapsingHeader,
		File,
		Bool
	};

	struct TENGINE FieldInfo
	{
		virtual ~FieldInfo() = default;
		FieldType type;
		std::string name;
	};

	struct TENGINE FieldNumber
	{
		virtual ~FieldNumber() = default;
		float minValue = 0.0f;
		float maxValue = 0.0f;
		std::function<void()> callback;
	};

	struct TENGINE FieldFloat : public FieldInfo, FieldNumber
	{
		FieldFloat()
		{
			type = FieldType::Float;
		}
		float* data = nullptr;
	};

	struct TENGINE FieldVec2 : public FieldInfo, FieldNumber
	{
		FieldVec2()
		{
			type = FieldType::Vec2;
		}
		Vec2* data = nullptr;
	};
	struct TENGINE FieldVec3 : public FieldInfo, FieldNumber
	{
		FieldVec3()
		{
			type = FieldType::Vec3;
		}
		Vec3* data = nullptr;
	};
	struct TENGINE FieldVec4 : public FieldInfo, FieldNumber
	{
		FieldVec4()
		{
			type = FieldType::Vec4;
		}

		Vec4* data = nullptr;
	};

	struct TENGINE FieldEnum : public FieldInfo
	{
		FieldEnum()
		{
			type = FieldType::Enum;
		}
		std::vector<std::string> elements;
		int* currentElement = nullptr;
		float minValue = 0;
		float maxValue = 0;
		std::function<void(int element)> callback;
	};

	struct TENGINE FieldCollapsingHeader : public FieldInfo
	{
		FieldCollapsingHeader()
		{
			type = FieldType::CollapsingHeader;
		}
		std::vector<std::shared_ptr<FieldInfo>> elements;
	};

	struct TENGINE FieldFile : public FieldInfo
	{
		FieldFile()
		{
			type = FieldType::File;
			path = "Null";
		}
		std::filesystem::path path;
		std::function<void(const std::string& path)> callback;
	};

	struct TENGINE FieldBool : public FieldInfo
	{
		FieldBool()
		{
			type = FieldType::Bool;
		}
		bool* data = nullptr;
		std::function<void()> callback;
	};

	class TENGINE ComponentInfo
	{
	public:
		ComponentInfo() = default;
		void addElement(std::shared_ptr<FieldInfo> element);
		void setComponentName(std::string_view name);
		std::string getComponentName() const;
		std::vector< std::shared_ptr<FieldInfo>> getElements() const;
	private:
		std::vector<std::shared_ptr<FieldInfo>> m_elements;
		std::string m_componentName;
	};
}
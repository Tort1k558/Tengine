#pragma once

#include<vector>
#include<functional>
#include<string>

#include"Renderer/Texture.h"

namespace Tengine
{
	enum class FieldType
	{
		None,
		Slider,
		Vec2,
		Vec3,
		Vec4,
		Enum,
		Image,
		CollapsingHeader,
		Button,
		File
	};

	struct FieldInfo
	{
		virtual ~FieldInfo() = default;
		FieldType type;
		std::string name;
	};

	struct FieldNumber
	{
		virtual ~FieldNumber() = default;
		float minValue;
		float maxValue;
		std::function<void()> callback;
	};

	struct FieldFloat : public FieldInfo, FieldNumber
	{
		float* data;
		FieldFloat()
		{
			type = FieldType::Slider;
		}
	};

	struct FieldVec2 : public FieldInfo, FieldNumber
	{
		Vec2* data;
		FieldVec2()
		{
			type = FieldType::Vec2;
		}
	};
	struct FieldVec3 : public FieldInfo, FieldNumber
	{
		Vec3* data;
		FieldVec3()
		{
			type = FieldType::Vec3;
		}
	};
	struct FieldVec4 : public FieldInfo, FieldNumber
	{
		Vec4* data;
		FieldVec4()
		{
			type = FieldType::Vec4;
		}
	};

	struct FieldEnum : public FieldInfo
	{
		FieldEnum()
		{
			type = FieldType::Enum;
		}
		std::vector<std::string> elements;
		int* currentElement;
		float minValue;
		float maxValue;
		std::function<void(int element)> callback;
	};

	struct FieldImage : public FieldInfo
	{
		FieldImage()
		{
			type = FieldType::Image;
		}
		std::shared_ptr<Texture> texture;
		Vec2 size;
	};

	struct FiledCollapsingHeader : public FieldInfo
	{
		FiledCollapsingHeader()
		{
			type = FieldType::CollapsingHeader;
		}
		std::vector<std::shared_ptr<FieldInfo>> elements;
	};

	struct FieldButton : public FieldInfo
	{
		FieldButton()
		{
			type = FieldType::Button;
		}
		std::function<void()> callback;
	};

	struct FieldFile : public FieldInfo
	{
		FieldFile()
		{
			type = FieldType::File;
		}
		std::filesystem::path path;
		std::function<void(const std::string& path)> callback;
	};

	class TENGINE ComponentInfo
	{
	public:
		ComponentInfo() = default;
		void addElement(std::shared_ptr<FieldInfo> element);
		void setComponentName(const std::string& name);
		std::string getComponentName() const;
		std::vector< std::shared_ptr<FieldInfo>> getElements() const;
	private:
		std::vector<std::shared_ptr<FieldInfo>> m_elements;
		std::string m_componentName;
	};
}
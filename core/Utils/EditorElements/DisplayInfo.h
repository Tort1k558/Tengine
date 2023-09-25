#pragma once

#include<vector>
#include<functional>
#include<string>

#include"Renderer/Texture.h"

namespace Tengine
{
	enum class DisplayTypeElement
	{
		None,
		Slider,
		Slider2,
		Slider3,
		Slider4,
		Combo,
		Image,
		CollapsingHeader,
		Button,
		FileDialog
	};

	struct DisplayInfoElement
	{
		virtual ~DisplayInfoElement() = default;
		DisplayTypeElement type;
		std::string name;
	};

	struct DisplayElementSlider
	{
		virtual ~DisplayElementSlider() = default;
		float minValue;
		float maxValue;
		std::function<void()> callback;
	};

	struct DisplayInfoElementSlider : public DisplayInfoElement, DisplayElementSlider
	{
		float* data;
		DisplayInfoElementSlider()
		{
			type = DisplayTypeElement::Slider;
		}
	};

	struct DisplayInfoElementSlider2 : public DisplayInfoElement, DisplayElementSlider
	{
		Vec2* data;
		DisplayInfoElementSlider2()
		{
			type = DisplayTypeElement::Slider2;
		}
	};
	struct DisplayInfoElementSlider3 : public DisplayInfoElement, DisplayElementSlider
	{
		Vec3* data;
		DisplayInfoElementSlider3()
		{
			type = DisplayTypeElement::Slider3;
		}
	};
	struct DisplayInfoElementSlider4 : public DisplayInfoElement, DisplayElementSlider
	{
		Vec4* data;
		DisplayInfoElementSlider4()
		{
			type = DisplayTypeElement::Slider4;
		}
	};

	struct DisplayInfoElementCombo : public DisplayInfoElement
	{
		DisplayInfoElementCombo()
		{
			type = DisplayTypeElement::Combo;
		}
		std::vector<std::string> elements;
		int* currentElement;
		float minValue;
		float maxValue;
		std::function<void(int element)> callback;
	};

	struct DisplayInfoElementImage : public DisplayInfoElement
	{
		DisplayInfoElementImage()
		{
			type = DisplayTypeElement::Image;
		}
		std::shared_ptr<Texture> texture;
		Vec2 size;
	};

	struct DisplayInfoElementCollapsingHeader : public DisplayInfoElement
	{
		DisplayInfoElementCollapsingHeader()
		{
			type = DisplayTypeElement::CollapsingHeader;
		}
		std::vector<std::shared_ptr<DisplayInfoElement>> elements;
	};

	struct DisplayInfoElementButton : public DisplayInfoElement
	{
		DisplayInfoElementButton()
		{
			type = DisplayTypeElement::Button;
		}
		std::function<void()> callback;
	};

	struct DisplayInfoElementFileDialog : public DisplayInfoElement
	{
		DisplayInfoElementFileDialog()
		{
			type = DisplayTypeElement::FileDialog;
		}
		std::filesystem::path path;
		std::function<void(const std::string& path)> callback;
	};

	class TENGINE ComponentInfo
	{
	public:
		ComponentInfo() = default;
		void addElement(std::shared_ptr<DisplayInfoElement> element);
		void setComponentName(const std::string& name);
		std::string getComponentName() const;
		std::vector< std::shared_ptr<DisplayInfoElement>> getElements() const;
	private:
		std::vector<std::shared_ptr<DisplayInfoElement>> m_elements;
		std::string m_componentName;
	};
}
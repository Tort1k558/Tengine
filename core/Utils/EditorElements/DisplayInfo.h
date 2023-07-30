#pragma once

#include<vector>
#include<functional>
#include<string>

#include"Renderer/Texture.h"

enum class DisplayTypeElement
{
	None,
	Slider,
	Slider2,
	Slider3,
	Slider4,
	Combo,
	Image,
	CollapsingHeader
};

struct DisplayInfoElement
{
	virtual ~DisplayInfoElement() = default;
	DisplayTypeElement type;
	std::string name;
	std::function<void()> callback;
};

struct DisplayElementSlider
{
	virtual ~DisplayElementSlider() = default;
	void* data;
	float minValue;
	float maxValue;
};

struct DisplayInfoElementSlider : public DisplayInfoElement, DisplayElementSlider
{
	DisplayInfoElementSlider()
	{
		type = DisplayTypeElement::Slider2;
	}
};

struct DisplayInfoElementSlider2 : public DisplayInfoElement, DisplayElementSlider
{
	DisplayInfoElementSlider2()
	{
		type = DisplayTypeElement::Slider2;
	}
};
struct DisplayInfoElementSlider3 : public DisplayInfoElement, DisplayElementSlider
{
	DisplayInfoElementSlider3()
	{
		type = DisplayTypeElement::Slider3;
	}
};
struct DisplayInfoElementSlider4 : public DisplayInfoElement, DisplayElementSlider
{
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
};

struct DisplayInfoElementImage : public DisplayInfoElement
{
	DisplayInfoElementImage()
	{
		type = DisplayTypeElement::Image;
	}
	std::shared_ptr<Texture> texture;
};

struct DisplayInfoElementCollapsingHeader : public DisplayInfoElement
{
	DisplayInfoElementCollapsingHeader()
	{
		type = DisplayTypeElement::CollapsingHeader;
	}
	std::vector<std::shared_ptr<DisplayInfoElement>> elements;
};

class DisplayInfo
{
public:
	DisplayInfo() = default;
	void addElement(std::shared_ptr<DisplayInfoElement> element);
	void setComponentName(const std::string& name);
	std::string getComponentName();
	std::vector< std::shared_ptr<DisplayInfoElement>> getElements();
private:
	std::vector<std::shared_ptr<DisplayInfoElement>> m_elements;
	std::string m_componentName;
};
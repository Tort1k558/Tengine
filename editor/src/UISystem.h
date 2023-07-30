#pragma once

#include<memory>

#include"Core/Window.h"
#include"ECS/System.h"
#include<Utils/EditorElements/DisplayInfo.h>

class UISystem : public System
{
public:
	void init() final;
	void update() final;
	void destroy() final;
	void setWindow(std::shared_ptr<Window> window);
private:
	void displayElement(std::shared_ptr<DisplayInfoElement> element);
	std::shared_ptr<Window> m_window;
};
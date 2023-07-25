#pragma once

#include<memory>

#include"Core/Window.h"
#include"ECS/System.h"

class UISystem : public System
{
public:
	void init() final;
	void update() final;
	void destroy() final;
	void setWindow(std::shared_ptr<Window> window);
private:
	std::shared_ptr<Window> m_window;
};
#pragma once

#include<memory>
#include<imgui/imgui.h>
#include<imgui/backends/imgui_impl_opengl3.h>
#include<imgui/backends/imgui_impl_glfw.h>

#include"Core/Window.h"

class UISystem
{
public:
	UISystem() = delete;
	UISystem(const UISystem&) = delete;
	UISystem& operator=(const UISystem&) = delete;
	
	static void Init();
	static void Update();
	static void SetWindow(std::shared_ptr<Window> window);
private:
	static std::shared_ptr<Window> m_window;
};
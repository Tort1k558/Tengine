#pragma once

#include"Core/Application.h"

#include"Components/Transform.h"
#include"Components/Camera.h"
#include"Components/Controller.h"
#include"Components/Mesh.h"
#include"Core/Timer.h"
#include"Core/Logger.h"
#include"Core/Input.h"
#include"Scene/SceneManager.h"
#include"Utils/Primitives.h"
#include"Core/AssetManager.h"

#include<imgui/imgui.h>
#include<imgui/backends/imgui_impl_opengl3.h>
#include<imgui/backends/imgui_impl_glfw.h>

class Editor : public Application
{
public:
    Editor(unsigned int width, unsigned int height, const std::string& title);
    void create() final;

    void update() final;
};
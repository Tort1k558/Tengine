#include "UISystem.h"

std::shared_ptr<Window> UISystem::m_window = nullptr;

void UISystem::Init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplGlfw_InitForOpenGL(m_window->getWindow(), true);
}

void UISystem::Update()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Text("Hello, world!");

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UISystem::SetWindow(std::shared_ptr<Window> window)
{
    m_window = window;
}

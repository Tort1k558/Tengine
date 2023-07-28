#include "UISystem.h"

#include<imgui/imgui.h>
#include<imgui/backends/imgui_impl_opengl3.h>
#include<imgui/backends/imgui_impl_glfw.h>

#include"Core/Timer.h"
#include"Core/Logger.h"
#include"ECS/Object.h"
#include"Scene/SceneManager.h"
void UISystem::init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsClassic();

    ImGui_ImplOpenGL3_Init();
    ImGui_ImplGlfw_InitForOpenGL(m_window->getWindow(), true);
}

void UISystem::update()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGuiWindowFlags windowsFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    windowsFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    windowsFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    windowsFlags |= ImGuiWindowFlags_NoBackground;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace", nullptr, windowsFlags);
    ImGui::PopStyleVar(3);

    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

    ImGui::BeginMenuBar();
    if (ImGui::BeginMenu("File"))
    {
        ImGui::MenuItem("hi",0);
        ImGui::EndMenu();
        ImGui::Separator();
    }
    ImGui::EndMenuBar();
    ImGui::End();

    ImGui::Begin("info", nullptr);
    ImGui::Text("FPS::%d", static_cast<unsigned int>(1.0 / Timer::GetDeltaTime()));
    ImGui::End();

    std::vector<std::shared_ptr<Object>> objects = SceneManager::GetCurrentScene()->getAllObjects();

    //Objects
    ImGui::Begin("Objects", nullptr);
    std::vector<std::string> objectNames;
    for (const auto& object : objects)
    {
        objectNames.push_back(object->getName());
    }
    std::sort(objectNames.begin(), objectNames.end(), std::less<std::string>());
    static int currentItem = 0;
    static std::string nameOfSelectedObject;
    if (ImGui::ListBox("##", &currentItem, [](void* data, int idx, const char** out_text) {
        auto& items = *static_cast<std::vector<std::string>*>(data);
        if (idx < 0 || idx >= static_cast<int>(items.size())) {
            *out_text = nullptr;
        }
        else {
            *out_text = items[idx].c_str();
        }
        return true;
        }, static_cast<void*>(&objectNames), static_cast<int>(objectNames.size())))
    {
        nameOfSelectedObject = objectNames[currentItem];
    }
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 25.0f);

    if (ImGui::Button("Create object",{0.0f,25.0f}))
    {
        Object::Create();
    }
    ImGui::End();


    //Components
    ImGui::Begin("Components", nullptr);
    if (!nameOfSelectedObject.empty())
    {
        std::shared_ptr<Object> object = SceneManager::GetCurrentScene()->getObjectByName(nameOfSelectedObject);
        std::vector<std::shared_ptr<Component>> components = object->getComponents();
        for (const auto& component : components)
        {
            if (component->hasDisplayInfo())
            {
                DisplayInfo info = component->getDisplayInfo();
                ImGui::Text(info.getComponentName().c_str());
                for (const auto& element : info.getElements())
                {
                    switch (element->type)
                    {
                    case DisplayTypeElement::None:
                        break;
                    case DisplayTypeElement::Slider:
                    {
                        std::shared_ptr<DisplayInfoElementSlider> slider = std::dynamic_pointer_cast<DisplayInfoElementSlider>(element);
                        if (ImGui::SliderFloat(slider->name.c_str(), static_cast<float*>(slider->data), slider->minValue, slider->maxValue))
                        {
                            if (slider->function)
                            {
                                slider->function();
                            }
                        }
                        break;
                    }
                    case DisplayTypeElement::Slider2:
                    {
                        std::shared_ptr<DisplayInfoElementSlider> slider = std::dynamic_pointer_cast<DisplayInfoElementSlider>(element);
                        ImGui::SliderFloat2(slider->name.c_str(), static_cast<float*>(slider->data), slider->minValue, slider->maxValue);
                        break;
                    }
                    case DisplayTypeElement::Slider3:
                    {
                        std::shared_ptr<DisplayInfoElementSlider> slider = std::dynamic_pointer_cast<DisplayInfoElementSlider>(element);
                        ImGui::SliderFloat3(slider->name.c_str(), static_cast<float*>(slider->data), slider->minValue, slider->maxValue);
                        break;
                    }
                    case DisplayTypeElement::Slider4:
                    {
                        std::shared_ptr<DisplayInfoElementSlider> slider = std::dynamic_pointer_cast<DisplayInfoElementSlider>(element);
                        ImGui::SliderFloat4(slider->name.c_str(), static_cast<float*>(slider->data), slider->minValue, slider->maxValue);
                        break;
                    }
                    case DisplayTypeElement::Combo:
                    {
                        std::shared_ptr<DisplayInfoElementCombo> combo = std::dynamic_pointer_cast<DisplayInfoElementCombo>(element);
                        if (ImGui::BeginCombo(combo->name.c_str(), combo->elements[*combo->currentElement].c_str()))
                        {
                            for (int i = 0; i < combo->elements.size(); ++i)
                            {
                                const bool isSelected = (*combo->currentElement == i);
                                if (ImGui::Selectable(combo->elements[i].c_str(), isSelected))
                                {
                                    *combo->currentElement = i;
                                    combo->function();
                                }

                                if (isSelected)
                                {
                                    ImGui::SetItemDefaultFocus();
                                }
                            }
                            ImGui::EndCombo();
                        }
                        break;
                    }
                    default:
                        break;
                    }
                }
            }
        }
    }
    ImGui::End();


    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_::ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* window = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(window);
    }
}

void UISystem::destroy()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UISystem::setWindow(std::shared_ptr<Window> window)
{
    m_window = window;
}

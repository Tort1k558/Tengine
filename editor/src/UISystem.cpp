#include "UISystem.h"

#include<imgui/imgui.h>
#include<imgui/backends/imgui_impl_opengl3.h>
#include<imgui/backends/imgui_impl_glfw.h>
#include<nfd.h>
#include"Core/Timer.h"
#include"Core/Logger.h"
#include"Components/Mesh.h"
#include"Components/Camera.h"
#include"Components/Script.h"
#include"ECS/Object.h"
#include"Scene/SceneManager.h"
#include"Scene/SceneSerializer.h"
#include"Core/AssetManager.h"
#include"Scripts/CodeGenerator.h"
#include"Systems/ScriptSystem.h"

using namespace Tengine;

std::shared_ptr<UISystem> UISystem::m_instance;

void UISystem::init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;


    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    style.WindowPadding = { 15, 15 };
    style.WindowRounding = 6.0f;
    style.FramePadding = { 5, 5 };
    style.FrameRounding = 4.0f;
    style.ItemSpacing = { 12, 8 };
    style.ItemInnerSpacing = { 8, 6 };
    style.IndentSpacing = 25.0f;
    style.ScrollbarSize = 15.0f;
    style.ScrollbarRounding = 9.0f;
    style.GrabMinSize = 5.0f;
    style.GrabRounding = 3.0f;
    style.WindowMenuButtonPosition = ImGuiDir_None;

    colors[ImGuiCol_Text] = { 0.80f, 0.80f, 0.83f, 1.00f };
    colors[ImGuiCol_TextDisabled] = { 0.24f, 0.23f, 0.29f, 1.00f };
    colors[ImGuiCol_WindowBg] = { 0.06f, 0.05f, 0.07f, 1.00f };
    colors[ImGuiCol_ChildBg] = { 0.07f, 0.07f, 0.09f, 1.00f };
    colors[ImGuiCol_PopupBg] = { 0.07f, 0.07f, 0.09f, 1.00f };
    colors[ImGuiCol_Border] = { 0.80f, 0.80f, 0.83f, 0.88f };
    colors[ImGuiCol_BorderShadow] = { 0.92f, 0.91f, 0.88f, 0.00f };
    colors[ImGuiCol_FrameBg] = { 0.10f, 0.09f, 0.12f, 1.00f };
    colors[ImGuiCol_FrameBgHovered] = { 0.24f, 0.23f, 0.29f, 1.00f };
    colors[ImGuiCol_FrameBgActive] = { 0.56f, 0.56f, 0.58f, 1.00f };
    colors[ImGuiCol_TitleBg] = { 0.76f, 0.31f, 0.00f, 1.00f };
    colors[ImGuiCol_TitleBgCollapsed] = { 1.00f, 0.98f, 0.95f, 0.75f };
    colors[ImGuiCol_TitleBgActive] = { 0.80f, 0.33f, 0.00f, 1.00f };
    colors[ImGuiCol_MenuBarBg] = { 0.10f, 0.09f, 0.12f, 1.00f };
    colors[ImGuiCol_ScrollbarBg] = { 0.10f, 0.09f, 0.12f, 1.00f };
    colors[ImGuiCol_ScrollbarGrab] = { 0.80f, 0.80f, 0.83f, 0.31f };
    colors[ImGuiCol_ScrollbarGrabHovered] = { 0.56f, 0.56f, 0.58f, 1.00f };
    colors[ImGuiCol_ScrollbarGrabActive] = { 0.06f, 0.05f, 0.07f, 1.00f };
    colors[ImGuiCol_CheckMark] = { 1.00f, 0.42f, 0.00f, 0.53f };
    colors[ImGuiCol_SliderGrab] = { 1.00f, 0.42f, 0.00f, 0.53f };
    colors[ImGuiCol_SliderGrabActive] = { 1.00f, 0.42f, 0.00f, 1.00f };
    colors[ImGuiCol_Button] = { 0.10f, 0.09f, 0.12f, 1.00f };
    colors[ImGuiCol_ButtonHovered] = { 0.24f, 0.23f, 0.29f, 1.00f };
    colors[ImGuiCol_ButtonActive] = { 0.56f, 0.56f, 0.58f, 1.00f };
    colors[ImGuiCol_Header] = { 0.10f, 0.09f, 0.12f, 1.00f };
    colors[ImGuiCol_HeaderHovered] = { 0.56f, 0.56f, 0.58f, 1.00f };
    colors[ImGuiCol_HeaderActive] = { 0.06f, 0.05f, 0.07f, 1.00f };
    colors[ImGuiCol_ResizeGrip] = { 0.00f, 0.00f, 0.00f, 0.00f };
    colors[ImGuiCol_ResizeGripHovered] = { 0.56f, 0.56f, 0.58f, 1.00f };
    colors[ImGuiCol_ResizeGripActive] = { 0.06f, 0.05f, 0.07f, 1.00f };
    colors[ImGuiCol_PlotLines] = { 0.40f, 0.39f, 0.38f, 0.63f };
    colors[ImGuiCol_PlotLinesHovered] = { 0.25f, 1.00f, 0.00f, 1.00f };
    colors[ImGuiCol_PlotHistogram] = { 0.40f, 0.39f, 0.38f, 0.63f };
    colors[ImGuiCol_PlotHistogramHovered] = { 0.25f, 1.00f, 0.00f, 1.00f };

    glfwInit();
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplGlfw_InitForOpenGL(m_window->getWindow(), true);
}

void UISystem::update()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGuiWindowFlags windowsFlags = ImGuiWindowFlags_MenuBar;
    windowsFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    windowsFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    windowsFlags |= ImGuiWindowFlags_NoBackground;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    //ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace", nullptr, windowsFlags);
    ImGui::PopStyleVar(3);

    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);


    static int currentItem = 0;
    static std::string nameOfSelectedObject;

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Scene")) {
            if (ImGui::MenuItem("Save Scene"))
            {
                SceneSerializer::Serialize(SceneManager::GetCurrentScene());
            }
            if (ImGui::MenuItem("Load Scene"))
            {
                nfdchar_t* outPath = nullptr;
                nfdresult_t result = NFD_OpenDialog(nullptr, nullptr, &outPath);
                if (result == NFD_OKAY) {
                    SceneSerializer::Deserialize(outPath);
                    nameOfSelectedObject.clear();
                }
            }
            if (ImGui::MenuItem("Create Scene")) 
            {
                SceneManager::SetCurrentScene(Scene::Create());
                nameOfSelectedObject.clear();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Script"))
        {
            if (ImGui::MenuItem("Compile scripts"))
            {
                CodeGenerator::CompileScripts();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::End();
    ImGui::Begin("info", nullptr);
    ImGui::Text("FPS::%d", static_cast<unsigned int>(1.0 / Timer::GetDeltaTime()));
    ImGui::End();


    //Objects
    std::vector<std::shared_ptr<Object>> objects = SceneManager::GetCurrentScene()->getAllObjects();

    ImGui::Begin("Objects", nullptr);
    std::vector<std::string> objectNames;
    for (const auto& object : objects)
    {
        objectNames.push_back(object->getName());
    }
    std::sort(objectNames.begin(), objectNames.end(), std::less<std::string>());

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

    if (ImGui::Button("Create object"))
    {
        Object::Create();
    }
    ImGui::SameLine();
    if (ImGui::Button("Delete object") && !nameOfSelectedObject.empty())
    {
        SceneManager::GetCurrentScene()->removeObjectByName(SceneManager::GetCurrentScene()->getObjectByName(nameOfSelectedObject)->getName());
        nameOfSelectedObject.clear();
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
            DisplayInfo info = component->getDisplayInfo();
            if (!info.getComponentName().empty())
            {
                if (ImGui::CollapsingHeader(info.getComponentName().c_str()))
                {
                    for (const auto& element : info.getElements())
                    {
                        displayElement(element);
                    }
                }
            }
        }
        if (ImGui::Button("Add component"))
        {
            ImGui::OpenPopup("Select Component");
        }
        if (ImGui::BeginPopup("Select Component"))
        {
            static int selectedItem = 0;
            std::vector<std::string> items = { "Mesh", "Camera"};
            for (const auto& scriptName : ScriptSystem::GetInstance()->getScriptNames())
            {
                items.push_back(scriptName);
            }
            if (ImGui::BeginCombo("##Components", items[selectedItem].c_str()))
            {
                for (int i = 0; i < items.size(); ++i)
                {
                    const bool isSelected = (selectedItem == i);
                    if (ImGui::Selectable(items[i].c_str(), isSelected))
                    {
                        selectedItem = i;
                    }

                    if (isSelected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }

            if (ImGui::Button("Add"))
            {
                if (selectedItem == 0)
                {
                    object->addComponent(Component::Create<Mesh>());
                }
                if (selectedItem == 1)
                {
                    object->addComponent(Component::Create<Camera>());
                }
                if (selectedItem >= 2)
                {
                    ScriptSystem::GetInstance()->addScript(object,items[selectedItem]);
                }
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }
    ImGui::End();

    //Render
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UISystem::destroy()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}

void UISystem::setWindow(std::shared_ptr<Window> window)
{
    m_window = window;
}

std::shared_ptr<UISystem> Tengine::UISystem::GetInstance()
{
    if (!m_instance)
    {
        m_instance = std::make_shared<UISystem>();
    }
    return m_instance;
}

void UISystem::displayElement(std::shared_ptr<DisplayInfoElement> element)
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
            if (slider->callback)
            {
                slider->callback();
            }
        }
        break;
    }
    case DisplayTypeElement::Slider2:
    {
        std::shared_ptr<DisplayInfoElementSlider2> slider = std::dynamic_pointer_cast<DisplayInfoElementSlider2>(element);
        ImGui::SliderFloat2(slider->name.c_str(), static_cast<float*>(slider->data), slider->minValue, slider->maxValue);
        break;
    }
    case DisplayTypeElement::Slider3:
    {
        std::shared_ptr<DisplayInfoElementSlider3> slider = std::dynamic_pointer_cast<DisplayInfoElementSlider3>(element);
        ImGui::SliderFloat3(slider->name.c_str(), static_cast<float*>(slider->data), slider->minValue, slider->maxValue);
        break;
    }
    case DisplayTypeElement::Slider4:
    {
        std::shared_ptr<DisplayInfoElementSlider4> slider = std::dynamic_pointer_cast<DisplayInfoElementSlider4>(element);
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
                    combo->callback(i);
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
    case DisplayTypeElement::Image:
    {
        std::shared_ptr<DisplayInfoElementImage> image = std::dynamic_pointer_cast<DisplayInfoElementImage>(element);
        ImGui::Text(image->name.c_str());
        if (image->texture)
        {
            ImGui::Image(reinterpret_cast<void*>(image->texture->getId()), { image->size.x,image->size.y });
        }
        else
        {
            ImGui::Text("No Texture");
        }
        break;
    }
    case DisplayTypeElement::CollapsingHeader:
    {
        std::shared_ptr<DisplayInfoElementCollapsingHeader> header = std::dynamic_pointer_cast<DisplayInfoElementCollapsingHeader>(element);
        if (ImGui::CollapsingHeader(header->name.c_str()))
        {
            for (size_t i = 0; i < header->elements.size(); i++)
            {
                displayElement(header->elements[i]);
            }
        };
        break;
    }
    case DisplayTypeElement::Button:
    {
        std::shared_ptr<DisplayInfoElementButton> button = std::dynamic_pointer_cast<DisplayInfoElementButton>(element);
        if (ImGui::Button(button->name.c_str()))
        {
            button->callback();
        };
        break;
    }
    case DisplayTypeElement::FileDialog:
    {
        std::shared_ptr<DisplayInfoElementFileDialog> fileDialog = std::dynamic_pointer_cast<DisplayInfoElementFileDialog>(element);

        if (ImGui::Button(fileDialog->name.c_str()))
        {
            nfdchar_t* outPath = nullptr;
            nfdresult_t result = NFD_OpenDialog(nullptr, nullptr, &outPath);
            if (result == NFD_OKAY) {
                fileDialog->callback(outPath);
            }
        };
        break;
    }
    default:
        break;
    }
}

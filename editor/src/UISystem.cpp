#include "UISystem.h"

#include<imgui/imgui.h>
#include<imgui/backends/imgui_impl_opengl3.h>
#include<imgui/backends/imgui_impl_glfw.h>
#include<nfd.h>

#include<Core/Timer.h>
#include<Core/Logger.h>
#include<Core/Input.h>
#include<Components/Mesh.h>
#include<Components/Camera.h>
#include<Components/Script.h>
#include<ECS/Object.h>
#include<ECS/SystemManager.h>
#include<Scene/SceneManager.h>
#include<Core/AssetManager.h>
#include<Systems/ScriptSystem.h>
#include<Systems/RendererSystem.h>

#include"Scripts/ScriptCompiler.h"
#include"Project.h"
#include"ProjectManager.h"
#include"ProjectBuilder.h"

namespace TengineEditor
{
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
        style.WindowMinSize = { 100.0f,100.0f };

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

        m_sceneFramebuffer = FrameBuffer::Create({ 1024,768 });
        m_sceneCamera = std::make_shared<Object>();
        m_sceneCamera->setName("SceneCamera");
        m_sceneCamera->addComponent(Component::Create<Transform>());
        m_sceneCamera->addComponent(Component::Create<Camera>());
    }

    void UISystem::update()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGuiWindowFlags windowsFlags = ImGuiWindowFlags_MenuBar;
        windowsFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
        windowsFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace", nullptr, windowsFlags);
        ImGui::PopStyleVar(3);

        ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

        renderMenuBar();
        renderWindowInfo();
        renderWindowObjects();
        renderWindowComponents();
        //Scene
        ImGui::Begin("Scene", nullptr);

        std::shared_ptr<Camera> sceneCamera = m_sceneCamera->getComponent<Camera>();
        std::shared_ptr<Transform> sceneCameraTransform = m_sceneCamera->getComponent<Transform>();
        if(ImGui::IsWindowFocused())
        {
            static float speed = 7.0f;
            static float cameraSensitivity = 0.314f;
            if (Input::IsKeyPressed(KeyCode::W))
            {
                sceneCameraTransform->setPosition(sceneCameraTransform->getPosition() + sceneCamera->getDirection() * Vec3(static_cast<float>(speed * Timer::GetDeltaTime())));
            }
            if (Input::IsKeyPressed(KeyCode::S))
            {
                sceneCameraTransform->setPosition(sceneCameraTransform->getPosition() + sceneCamera->getDirection() * Vec3(static_cast<float>(-speed * Timer::GetDeltaTime())));
            }
            if (Input::IsKeyPressed(KeyCode::A))
            {
                sceneCameraTransform->setPosition(sceneCameraTransform->getPosition() + Normalize(Cross(sceneCamera->getDirection(), sceneCamera->getUp()))
                    * Vec3(static_cast<float>(-speed * Timer::GetDeltaTime())));
            }
            if (Input::IsKeyPressed(KeyCode::D))
            {
                sceneCameraTransform->setPosition(sceneCameraTransform->getPosition() + Normalize(Cross(sceneCamera->getDirection(), sceneCamera->getUp()))
                    * Vec3(static_cast<float>(speed * Timer::GetDeltaTime())));
            }
            if (Input::IsKeyPressed(KeyCode::LEFT_SHIFT))
            {
                sceneCameraTransform->setPosition(sceneCameraTransform->getPosition() + Vec3(0.0f, static_cast<float>(-speed * Timer::GetDeltaTime()), 0.0f));
            }
            if (Input::IsKeyPressed(KeyCode::SPACE))
            {
                sceneCameraTransform->setPosition(sceneCameraTransform->getPosition() + Vec3(0.0f, static_cast<float>(speed * Timer::GetDeltaTime()), 0.0f));
            }
            if (Input::IsMouseButtonPressed(MouseButton::BUTTON_2))
            {
                static Vec2 deltaMouse(0.0f, 0.0f);
                if (deltaMouse.x != 0)
                {
                    sceneCameraTransform->setRotationY(sceneCameraTransform->getRotation().y + deltaMouse.x * cameraSensitivity);
                }
                if (deltaMouse.y != 0)
                {
                    if (sceneCameraTransform->getRotation().x + deltaMouse.y * cameraSensitivity < 89.0f && sceneCameraTransform->getRotation().x + deltaMouse.y * cameraSensitivity > -89.0f)
                    {
                        sceneCameraTransform->setRotationX(sceneCameraTransform->getRotation().x + deltaMouse.y * cameraSensitivity);
                    }
                }
                deltaMouse = Input::GetDeltaMousePosition();
            }
        }
        m_sceneFramebuffer->bind();
        RendererSystem::GetInstance()->getRendererContext()->clear();
        RendererSystem::GetInstance()->getRendererContext()->clearColor({ 0.1f,0.1f,0.1f,1.0f });
        RendererSystem::GetInstance()->renderCamera(m_sceneCamera->getComponent<Camera>());
        m_sceneFramebuffer->unbind();

        ImVec2 availableAreaSceneWindow = ImGui::GetContentRegionAvail();
        std::shared_ptr<Texture> sceneTexture = m_sceneFramebuffer->getColorTexture();
        ImGui::Image((void*)sceneTexture->getId(), availableAreaSceneWindow, { 0, 1 }, { 1, 0 });
        if (UVec2(availableAreaSceneWindow.x,availableAreaSceneWindow.y) != m_sceneFramebuffer->getSize())
        {
            m_sceneFramebuffer = FrameBuffer::Create({ availableAreaSceneWindow.x,availableAreaSceneWindow.y });
        }
        ImGui::End();

        //Game
        static bool isGameRunning = false;
        ImGui::Begin("Game", nullptr);
        if (isGameRunning) {
            if (ImGui::Button("stop")) {
                isGameRunning = false;
                SystemManager::DestroySystems();
                SceneManager::LoadByPath(SceneManager::GetCurrentScene()->getPath());
            }
        }
        else {
            if (ImGui::Button("start")) {
                isGameRunning = true;
                SceneManager::Save(SceneManager::GetCurrentScene());
                SystemManager::AddSystem(ScriptSystem::GetInstance());
                SceneManager::LoadByPath(SceneManager::GetCurrentScene()->getPath());
                SystemManager::InitSystems();
            }
        } 
        ImVec2 availableAreaGameWindow = ImGui::GetContentRegionAvail();
        std::shared_ptr<Texture> gameTexture = RendererSystem::GetInstance()->getFramebuffer()->getColorTexture();
        ImGui::Image((void*)gameTexture->getId(), availableAreaGameWindow, { 0, 1 }, { 1, 0 });
        RendererSystem::GetInstance()->updateViewport({ availableAreaGameWindow.x, availableAreaGameWindow.y });

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

    std::shared_ptr<UISystem> UISystem::GetInstance()
    {
        if (!m_instance)
        {
            m_instance = std::make_shared<UISystem>();
        }
        return m_instance;
    }

    void UISystem::displayElement(std::shared_ptr<FieldInfo> element)
    {
        switch (element->type)
        {
        case FieldType::None:
            break;
        case FieldType::Float:
        {
            std::shared_ptr<FieldFloat> slider = std::dynamic_pointer_cast<FieldFloat>(element);
            if (ImGui::DragFloat(slider->name.c_str(), static_cast<float*>(slider->data),0.0f ,slider->minValue, slider->maxValue))
            {
                if (slider->callback)
                {
                    slider->callback();
                }
            }
            break;
        }
        case FieldType::Vec2:
        {
            std::shared_ptr<FieldVec2> slider = std::dynamic_pointer_cast<FieldVec2>(element);
            ImGui::DragFloat2(slider->name.c_str(), static_cast<float*>(&slider->data->x), 0.0f, slider->minValue, slider->maxValue);
            break;
        }
        case FieldType::Vec3:
        {
            std::shared_ptr<FieldVec3> slider = std::dynamic_pointer_cast<FieldVec3>(element);
            ImGui::DragFloat3(slider->name.c_str(), static_cast<float*>(&slider->data->x), 0.0f, slider->minValue, slider->maxValue);
            break;
        }
        case FieldType::Vec4:
        {
            std::shared_ptr<FieldVec4> slider = std::dynamic_pointer_cast<FieldVec4>(element);
            ImGui::DragFloat4(slider->name.c_str(), static_cast<float*>(&slider->data->x), 0.0f, slider->minValue, slider->maxValue);
            break;
        }
        case FieldType::Enum:
        {
            std::shared_ptr<FieldEnum> combo = std::dynamic_pointer_cast<FieldEnum>(element);
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
        case FieldType::Image:
        {
            std::shared_ptr<FieldImage> image = std::dynamic_pointer_cast<FieldImage>(element);
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
        case FieldType::CollapsingHeader:
        {
            std::shared_ptr<FiledCollapsingHeader> header = std::dynamic_pointer_cast<FiledCollapsingHeader>(element);
            if (ImGui::CollapsingHeader(header->name.c_str()))
            {
                for (size_t i = 0; i < header->elements.size(); i++)
                {
                    displayElement(header->elements[i]);
                }
            };
            break;
        }
        case FieldType::Button:
        {
            std::shared_ptr<FieldButton> button = std::dynamic_pointer_cast<FieldButton>(element);
            if (ImGui::Button(button->name.c_str()))
            {
                button->callback();
            };
            break;
        }
        case FieldType::File:
        {
            std::shared_ptr<FieldFile> fileDialog = std::dynamic_pointer_cast<FieldFile>(element);

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

    void UISystem::renderMenuBar()
    {
        static bool isOpenedProjectSettings = false;
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("Project"))
            {
                if (ImGui::MenuItem("Save Project"))
                {
                    ProjectManager::Save();
                }
                if (ImGui::MenuItem("Load Project"))
                {
                    nfdchar_t* outPath = nullptr;
                    nfdresult_t result = NFD_OpenDialog("project", nullptr, &outPath);
                    if (result == NFD_OKAY) {
                        ProjectManager::Load(outPath);
                    }
                }
                if (ImGui::MenuItem("Create Project"))
                {
                    nfdchar_t* outPath = nullptr;
                    nfdresult_t result = NFD_PickFolder(nullptr, &outPath);
                    if (result == NFD_OKAY) {
                        ProjectManager::Create(outPath);
                    }
                }
                if (ImGui::MenuItem("Project Settings"))
                {
                    isOpenedProjectSettings = true;
                }
                if (ImGui::MenuItem("Build Project"))
                {
                    ProjectManager::Save();
                    ProjectBuilder::Build();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Scene")) {
                if (ImGui::MenuItem("Save Scene"))
                {
                    SceneManager::Save(SceneManager::GetCurrentScene());
                }
                if (ImGui::MenuItem("Load Scene"))
                {
                    nfdchar_t* outPath = nullptr;
                    nfdresult_t result = NFD_OpenDialog(nullptr, nullptr, &outPath);
                    if (result == NFD_OKAY) {
                        SceneManager::LoadByPath(outPath);
                        m_nameOfSelectedObject.clear();
                    }
                }
                if (ImGui::MenuItem("Create Scene"))
                {
                    std::shared_ptr<Scene> scene = Scene::Create();
                    scene->setPath(ProjectManager::GetInstance()->getPath().string() + "/" + scene->getName());
                    SceneManager::SetCurrentScene(scene);
                    m_nameOfSelectedObject.clear();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Script"))
            {
                if (ImGui::MenuItem("Compile scripts"))
                {
                    ScriptCompiler::Compile();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        if (isOpenedProjectSettings)
        {
            if (ImGui::Begin("Project Settings", &isOpenedProjectSettings, ImGuiWindowFlags_NoCollapse))
            {
                float windowWidth = ImGui::GetWindowWidth();

                ImGui::Columns(2);
                ImGui::SetColumnWidth(0, windowWidth * 0.3f);
                static std::string selectedMenu;
                if (ImGui::BeginListBox("##List", ImVec2(-1, -1)))
                {
                    if (ImGui::Selectable("Build", true))
                    {
                        selectedMenu = "Build";
                    }
                    
                    if (ImGui::Selectable("Render", false))
                    {
                        selectedMenu = "Render";
                    }

                    ImGui::EndListBox();
                }
                
                ImGui::NextColumn();
                
                if (selectedMenu == "Build")
                {
                    std::vector<std::filesystem::path> pathToScenes = ProjectManager::GetInstance()->getPathToScenes();
                    static int draggedStringIndex = -1;
                    for (int i = 0; i < pathToScenes.size(); ++i) {
                        ImGui::Button(pathToScenes[i].string().c_str());

                        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
                            ImGui::SetDragDropPayload("DND_STRING", &i, sizeof(int));
                            ImGui::Text("%s", pathToScenes[i].string().c_str());
                            ImGui::EndDragDropSource();
                        }
                        if (ImGui::BeginDragDropTarget()) {
                            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_STRING")) {
                                IM_ASSERT(payload->DataSize == sizeof(int));
                                int dropped_idx = *(const int*)payload->Data;
                                if (dropped_idx != i)
                                {
                                    ProjectManager::GetInstance()->swapScenes(i, dropped_idx);
                                }
                            }
                            ImGui::EndDragDropTarget();
                        }

                        ImGui::SameLine();
                        ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - ImGui::CalcTextSize("Delete").x);
                        if (ImGui::Button("Delete##"))
                        {
                            ProjectManager::GetInstance()->removeScene(pathToScenes[i].string());
                            --i;
                        }
                    }

                    static std::string newString;
                    if (ImGui::InputText("New Scene", newString.data(), 1024, ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        ProjectManager::GetInstance()->addScene(newString.data());
                    }
                    static BuildConfiguration buildConfiguration = BuildConfiguration::Debug;
                    const char* buildConfigurations[] = { "Debug","Release" };
                    if (ImGui::BeginCombo("BuildConfiguration", buildConfigurations[static_cast<int>(buildConfiguration)]))
                    {
                        for (int i = 0; i < 2; i++)
                        {
                            const bool isSelected = (static_cast<int>(buildConfiguration) == i);
                            if (ImGui::Selectable(buildConfigurations[i], isSelected))
                            {
                                buildConfiguration = static_cast<BuildConfiguration>(i);
                                ProjectBuilder::SetBuildConfiguration(buildConfiguration);
                            }

                            if (isSelected)
                            {
                                ImGui::SetItemDefaultFocus();
                            }
                        }
                        ImGui::EndCombo();
                    }

                }
                else if (selectedMenu == "Render")
                {

                }
                ImGui::End();
            }
        }
    }

    void UISystem::renderWindowObjects() 
    {

        static int currentItem = 0;
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
            m_nameOfSelectedObject = objectNames[currentItem];
        }
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 25.0f);


        if (ImGui::Button("Create object"))
        {
            Object::Create();
        }
        ImGui::SameLine();
        if (ImGui::Button("Delete object") && !m_nameOfSelectedObject.empty())
        {
            SceneManager::GetCurrentScene()->removeObjectByName(SceneManager::GetCurrentScene()->getObjectByName(m_nameOfSelectedObject)->getName());
            m_nameOfSelectedObject.clear();
        }
        ImGui::End();
    }

    void UISystem::renderWindowComponents()
    {
        ImGui::Begin("Components", nullptr);
        if (!m_nameOfSelectedObject.empty())
        {
            bool contextMenuOpened = false;
            static std::shared_ptr<Component> selectedComponent;
            std::shared_ptr<Object> object = SceneManager::GetCurrentScene()->getObjectByName(m_nameOfSelectedObject);
            std::vector<std::shared_ptr<Component>> components = object->getComponents();
            for (const auto& component : components)
            {
                ComponentInfo info = component->getInfo();
                if (!info.getComponentName().empty())
                {
                    if (ImGui::CollapsingHeader(info.getComponentName().c_str()))
                    {
                        for (const auto& element : info.getElements())
                        {
                            displayElement(element);
                        }
                    }
                    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(1))
                    {
                        contextMenuOpened = true;
                        selectedComponent = component;
                    }
                }
            }
            if (contextMenuOpened)
            {
                ImGui::OpenPopup("Component Menu");
                contextMenuOpened = false;
            }

            if (ImGui::BeginPopup("Component Menu"))
            {
                if (ImGui::MenuItem("Delete Component"))
                {
                    if (selectedComponent)
                    {
                        object->removeComponent(selectedComponent);
                        selectedComponent.reset();
                    }
                }
                ImGui::EndPopup();
            }

            if (ImGui::Button("Add component"))
            {
                ImGui::OpenPopup("Select Component");
            }
            if (ImGui::BeginPopup("Select Component"))
            {
                static int selectedItem = 0;
                std::vector<std::string> items = { "Mesh", "Camera" };
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
                        ScriptSystem::GetInstance()->addScript(object, items[selectedItem]);
                    }
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
        ImGui::End();
    }

    void UISystem::renderWindowInfo()
    {
        ImGui::End();
        ImGui::Begin("info", nullptr);
        ImGui::Text("FPS::%d", static_cast<unsigned int>(1.0 / Timer::GetDeltaTime()));
        ImGui::End();
    }

}
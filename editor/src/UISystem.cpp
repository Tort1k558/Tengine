#include "UISystem.h"

#include<functional>

#include<imgui/backends/imgui_impl_glfw.h>
#include<imgui/backends/imgui_impl_opengl3.h>
#include<imgui/imgui.h>
#include<imgui/misc/cpp/imgui_stdlib.h>
#include<imguizmo/ImGuizmo.h>
#include<nfd.h>

#include<Components/Camera.h>
#include<Components/Model.h>
#include<Core/AssetManager.h>
#include<Core/Input.h>
#include<Core/Timer.h>
#include<ECS/Object.h>
#include<ECS/SystemManager.h>
#include<Scene/SceneManager.h>
#include<Systems/RendererSystem.h>
#include<Systems/ScriptSystem.h>
#include<Utils/Material.h>

#include"UIRender.h"
#include"FileManager.h"
#include"ProjectBuilder.h"
#include"ProjectManager.h"
#include"Scripts/ScriptCompiler.h"
#include"Windows/WindowMonitor.h"
#include"Windows/WindowObjects.h"
#include"Windows/WindowFileBrowser.h"

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
        colors[ImGuiCol_WindowBg] = { 0.1f, 0.1f, 0.1f, 1.00f };
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

        WindowMonitor::AddFormatHandler(".material",[](std::filesystem::path path) {
            std::shared_ptr<Material> material = AssetManager::LoadMaterial(path);
            if (material)
            {
                auto checkSubMaterial = [&material](std::string subMaterialName, SubMaterialType type)
                    {
                        ImGui::PushID(subMaterialName.c_str());
                        if (material->hasSubMaterial(type))
                        {
                            std::shared_ptr<SubMaterial> subMaterial = material->getSubMaterial(type);
                            std::string texturePath;
                            if (subMaterial->hasTexture())
                            {
                                texturePath = subMaterial->getTexture()->getPath().string();
                            }
                            if (UIRender::DrawFile(subMaterialName, texturePath))
                            {
                                *subMaterial = *(std::make_shared<SubMaterial>(AssetManager::LoadTexture(texturePath)));
                            }
                            if (subMaterial->hasTexture())
                            {
                                UIRender::DrawImage(reinterpret_cast<void*>(subMaterial->getTexture()->getId()), { 75,75 });
                            }
                            Vec3 color = subMaterial->getColor();
                            if (ImGui::ColorEdit3(subMaterialName.c_str(), &(color)[0]))
                            {
                                subMaterial->setColor(color);
                            }
                            if (UIRender::DrawButton("Delete"))
                            {
                                material->removeSubMaterial(type);
                            }
                        }
                        else
                        {
                            ImGui::Text(subMaterialName.c_str());
                            ImGui::Text("None");
                            ImGui::SameLine();
                            if (UIRender::DrawButton("Create"))
                            {
                                material->setSubMaterial(type, std::make_shared<SubMaterial>());
                            }
                        }
                        ImGui::PopID();
                    };

                checkSubMaterial("Albedo", SubMaterialType::Diffuse);
                checkSubMaterial("Normal", SubMaterialType::Normal);
                checkSubMaterial("Specular", SubMaterialType::Specular);
                checkSubMaterial("Height", SubMaterialType::Height);
                checkSubMaterial("Roughness", SubMaterialType::Roughness);
            }
            });

        WindowMonitor::AddFormatHandler(".model", [](std::filesystem::path path) 
            {
                std::shared_ptr<Model> model = AssetManager::LoadModel(path);
                std::string meshPath = model->getMesh()->getPath().string();
                if (UIRender::DrawFile("PathToMesh", meshPath))
                {
                    *model = *AssetManager::CreateModel(meshPath);
                }

                if (UIRender::DrawCollapsingHeader("SubMeshes"))
                {
                    if (model->getMesh())
                    {
                        std::vector<std::shared_ptr<SubMesh>> submeshes = model->getMesh()->getSubmeshes();
                        for (size_t i = 0; i < submeshes.size(); i++)
                        {
                            if (UIRender::DrawCollapsingHeader("SubMesh" + std::to_string(i)))
                            {
                                std::string submeshMaterialPath;
                                if (model->hasSubmeshMaterial(i))
                                {
                                    submeshMaterialPath = model->getSubmeshMaterial(i)->getPath().string();
                                }
                                if (UIRender::DrawFile("SubMesh Material" + std::to_string(i), submeshMaterialPath))
                                {
                                    model->setSubmeshMaterial(i, AssetManager::LoadMaterial(submeshMaterialPath));
                                }
                            }
                        }
                    }
                }
            });

        WindowMonitor::AddFormatHandler(".cubeMap", [](std::filesystem::path path) {
            std::shared_ptr<CubeMapTexture> cubeMap = AssetManager::LoadCubeMapTexture(path);
            if (cubeMap)
            {
                auto drawTexture = [&cubeMap](std::string name, CubeMapSide side)
                    {
                        std::string pathToTexture = cubeMap->getSupportingInfo(name);
                        if (UIRender::DrawFile(name, pathToTexture))
                        {
                            cubeMap->setTexture(side, AssetManager::LoadTexture(pathToTexture));
                            cubeMap->setSupportingInfo(name, pathToTexture);
                            AssetManager::SaveCubeMapTexture(cubeMap.get());
                        }
                    };
                drawTexture("right", CubeMapSide::Right);
                drawTexture("left", CubeMapSide::Left);
                drawTexture("top", CubeMapSide::Top);
                drawTexture("bottom", CubeMapSide::Bottom);
                drawTexture("front", CubeMapSide::Front);
                drawTexture("back", CubeMapSide::Back);
            }
            });
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
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        renderMenuBar();
        renderWindowInfo();
        WindowObjects::Render();
        WindowMonitor::Render();
        WindowFileBrowser::Render();
        
        

        //Scene
        ImGui::Begin("Scene", nullptr);

        static ImGuizmo::OPERATION gizmoOperation = ImGuizmo::TRANSLATE;
        static ImGuizmo::MODE gizmoMode = ImGuizmo::WORLD;
        if (UIRender::DrawButton("Translate"))
        {
            gizmoOperation = ImGuizmo::TRANSLATE;
            gizmoMode = ImGuizmo::WORLD;
        }
        ImGui::SameLine();
        if (UIRender::DrawButton("Rotate"))
        {
            gizmoOperation = ImGuizmo::ROTATE;
            gizmoMode = ImGuizmo::LOCAL;
        }
        ImGui::SameLine();
        if (UIRender::DrawButton("Scale"))
        {
            gizmoOperation = ImGuizmo::SCALE;
            gizmoMode = ImGuizmo::LOCAL;
        }
        if (ImGui::IsKeyPressed(ImGuiKey_Q))
        {
            gizmoOperation = ImGuizmo::TRANSLATE;
            gizmoMode = ImGuizmo::WORLD;
        }
        if (ImGui::IsKeyPressed(ImGuiKey_E))
        {
            gizmoOperation = ImGuizmo::ROTATE;
            gizmoMode = ImGuizmo::LOCAL;
        }
        if (ImGui::IsKeyPressed(ImGuiKey_R))
        {
            gizmoOperation = ImGuizmo::SCALE;
            gizmoMode = ImGuizmo::LOCAL;
        }

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
                sceneCameraTransform->setPosition(sceneCameraTransform->getPosition() + Math::Normalize(Math::Cross(sceneCamera->getDirection(), sceneCamera->getUp()))
                    * Vec3(static_cast<float>(-speed * Timer::GetDeltaTime())));
            }
            if (Input::IsKeyPressed(KeyCode::D))
            {
                sceneCameraTransform->setPosition(sceneCameraTransform->getPosition() + Math::Normalize(Math::Cross(sceneCamera->getDirection(), sceneCamera->getUp()))
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
        

        //Gizmo
        ImVec2 sceneWindowPosition = ImGui::GetWindowPos();
        ImVec2 sceneWindowSize = ImGui::GetWindowSize();

        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(sceneWindowPosition.x, sceneWindowPosition.y, sceneWindowSize.x, sceneWindowSize.y);
        ImGuizmo::AllowAxisFlip(false);
        if (WindowObjects::GetSelectedObject())
        {
            Mat4 view = sceneCamera->getViewMatrix();
            Mat4 projection = sceneCamera->getProjection()->getProjectionMatrix();
            std::shared_ptr<Transform> transformSelected = WindowObjects::GetSelectedObject()->getComponent<Transform>();
            Mat4 model(1.0f);
            ImGuizmo::RecomposeMatrixFromComponents(&transformSelected->getPosition()[0], &transformSelected->getRotation()[0],
                &transformSelected->getScale()[0], &model[0][0]);

            if (ImGuizmo::Manipulate(&view[0][0], &projection[0][0], gizmoOperation, gizmoMode, &model[0][0]))
            {
                Vec3 transform, rotation, scale;
                ImGuizmo::DecomposeMatrixToComponents(&model[0][0], &transform[0], &rotation[0], &scale[0]);
                transformSelected->setPosition(transform);
                transformSelected->setRotation(rotation);
                transformSelected->setScale(scale);
            }
        }
        ImGui::End();


        //Game
        static bool isGameRunning = false;
        ImGui::Begin("Game", nullptr);
        if (isGameRunning) {
            if (ImGui::Button("stop")) 
            {
                isGameRunning = false;
                SystemManager::DestroySystems();
                SceneManager::LoadByPath(SceneManager::GetCurrentScene()->getPath());
            }
        }
        else {
            if (ImGui::Button("start")) 
            {
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
                        WindowObjects::SetSelectedObject(nullptr);
                    }
                }
                if (ImGui::MenuItem("Create Scene"))
                {
                    std::shared_ptr<Scene> scene = Scene::Create();
                    scene->setPath(ProjectManager::GetInstance()->getPath().string() + "/" + scene->getName());
                    SceneManager::SetCurrentScene(scene);
                    WindowObjects::SetSelectedObject(nullptr);
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

                if (ImGui::BeginTable("##ProjectSettigns", 2, ImGuiTableFlags_Resizable))
                {
                    ImGui::TableSetupColumn("##ListOfSettings", ImGuiTableColumnFlags_WidthFixed, windowWidth * 0.3f);

                    ImGui::TableNextColumn();
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

                        if (ImGui::Selectable("Scripts", false))
                        {
                            selectedMenu = "Scripts";
                        }
                        ImGui::EndListBox();
                    }

                    ImGui::TableNextColumn();

                    if (selectedMenu == "Build")
                    {
                        std::vector<std::filesystem::path> pathToScenes = ProjectManager::GetInstance()->getPathToScenes();
                        static int draggedStringIndex = -1;
                        for (int i = 0; i < pathToScenes.size(); ++i) {
                            ImGui::Button(pathToScenes[i].string().c_str());

                            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) 
                            {
                                ImGui::SetDragDropPayload("swapIndex", &i, sizeof(int));
                                ImGui::Text("%s", pathToScenes[i].string().c_str());
                                ImGui::EndDragDropSource();
                            }

                            if (ImGui::BeginDragDropTarget()) {
                                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("swapIndex")) 
                                {
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
                            ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - ImGui::CalcTextSize("Delete").x - 5.0f);
                            if (ImGui::Button("Delete##"))
                            {
                                ProjectManager::GetInstance()->removeSceneByPath(pathToScenes[i]);
                                --i;
                            }
                        }

                        static std::string newString;
                        if (ImGui::InputText("New Scene", newString.data(), 1024, ImGuiInputTextFlags_EnterReturnsTrue))
                        {
                            ProjectManager::GetInstance()->addScene(newString.data());
                            newString = "";
                        }

                        if (ImGui::BeginDragDropTarget()) 
                        {
                            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("path")) 
                            {
                                std::string path(static_cast<const char*>(payload->Data), payload->DataSize);
                                if (path != "")
                                {
                                    ProjectManager::GetInstance()->addScene(path);
                                }
                            }
                            ImGui::EndDragDropTarget();
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

                        static BuildPlatform buildPlatform = BuildPlatform::Windows;
                        const char* buildPlatforms[] = { "Windows", "HTML5" };
                        if (ImGui::BeginCombo("BuildPlatform", buildPlatforms[static_cast<int>(buildPlatform)]))
                        {
                            for (int i = 0; i < 2; i++)
                            {
                                const bool isSelected = (static_cast<int>(buildPlatform) == i);
                                if (ImGui::Selectable(buildPlatforms[i], isSelected))
                                {
                                    buildPlatform = static_cast<BuildPlatform>(i);
                                    ProjectBuilder::SetBuildPlatform(buildPlatform);
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
                    else if (selectedMenu == "Scripts")
                    {
                        static BuildConfiguration buildScriptConfiguration = BuildConfiguration::Debug;
                        const char* buildConfigurations[] = { "Debug","Release" };
                        if (ImGui::BeginCombo("BuildConfiguration", buildConfigurations[static_cast<int>(buildScriptConfiguration)]))
                        {
                            for (int i = 0; i < 2; i++)
                            {
                                const bool isSelected = (static_cast<int>(buildScriptConfiguration) == i);
                                if (ImGui::Selectable(buildConfigurations[i], isSelected))
                                {
                                    buildScriptConfiguration = static_cast<BuildConfiguration>(i);
                                    ScriptCompiler::SetScriptBuildConfiguration(buildScriptConfiguration);
                                }

                                if (isSelected)
                                {
                                    ImGui::SetItemDefaultFocus();
                                }
                            }
                            ImGui::EndCombo();
                        }
                    }
                    ImGui::EndTable();
                }
                ImGui::End();
            }
        }
    }

    void UISystem::renderWindowInfo()
    {
        ImGui::End();
        ImGui::Begin("info", nullptr);
        ImGui::Text("FPS::%d", static_cast<unsigned int>(1.0 / Timer::GetDeltaTime()));
        ImGui::End();
    }
}